local functional   = require 'std.functional'
local EaseFunction = require 'Ease.EaseFunction'
local SpanVariable = require 'Action.SpanVariable'
local SmoothAiming = require 'Camera.SmoothAiming'
local DirectAvoid  = require 'Camera.DirectAvoid'
local DirectFollow = require 'Camera.DirectFollow'
local ActionAdapter= require 'Action.ActionAdapter'
local ModifierAdapter = require 'Modifier.ModifierAdapter'
local ModifierFactory = require 'Modifier.TargetModifierFactory'
local AsConditionFactory= require 'Async.AsConditionFactory'
local CameraMovement	= require 'HighAction.CameraMovement'
local CameraFacade = {
	offsetSourceVec= math3d.vector(8, 20, 10),
	translateSpeed = 90, 
	slideSpeed = 60,
	slideEase  = EaseFunction.wrapInOut(EaseFunction[3]),
	
	cameraObject  = nil,
	followControl = nil,
	aimingControl = nil,
	cameraAvoid   = nil,
	
	focusedObject = nil,
	spanTranslate = nil,
	spanLookDir   = nil,
}
CameraFacade.__index = CameraFacade

function CameraFacade.new(cameraObject)
	local cameraControl = cameraObject:findComponent(c'Camera')
	local following = {
		getTranslate = functional.bindself(cameraControl, 'getTarget'),
	}
	
	local o = setmetatable({
			cameraObject  = cameraObject,
			aimingControl = SmoothAiming.new(cameraObject, math3d.vector(0.5, -0.5, 0.5)),
			followControl = DirectFollow.new(cameraObject),
			cameraAvoid   = DirectAvoid.new(cameraObject, following),
		}, CameraFacade)
	
	o.followControl:setFollowing(following)
	
	ModifierAdapter.fit(cameraObject, o.cameraAvoid)
	ModifierAdapter.fit(cameraObject, o.followControl)
	ModifierAdapter.fit(cameraObject, o.aimingControl)
	
	return o
end

function CameraFacade:focus(object)
	self.focusedObject = object
	self.aimingControl:setAim(object)
	AsConditionFactory.waitCameraAim(self.aimingControl)
end

function CameraFacade:_setSmartCameraEnabled(b)
	self.cameraAvoid:setEnabled(b)
	self.followControl:setEnabled(b)
	self.aimingControl:setEnabled(b)
end

function CameraFacade:_calculateTranslate(newTarget)
	local yDirection = math3d.vector(0, 1, 0)
	
	local oldTarget = self.focusedObject:getTranslate()
	local zDirection= oldTarget - newTarget
	zDirection:set(nil, 0, nil)
	zDirection:normalize()
	
	local xDirection= zDirection:cross(yDirection)
	xDirection:normalize()
	
	local cameraPosition = self.cameraObject:getTranslate()
	local toCamera = cameraPosition - oldTarget
	
	if toCamera:dot(xDirection) < 0 then
		xDirection = -xDirection
	end
	
	local x, y, z = self.offsetSourceVec:xyz()
	return oldTarget + xDirection*x + yDirection*y + zDirection*z
end

function CameraFacade:_waitCameraMove()
	local action = CameraMovement.slide(self.cameraObject, 
		self.spanTranslate, self.spanLookDir, self.slideSpeed, self.slideEase)
	
	ActionAdapter.fit(self.cameraObject, action)
	AsConditionFactory.waitAction(action)
end

function CameraFacade:descendIntoBattle(targetTile)
	assert(self.focusedObject, 'Illegal state to descend into battle')
	
	self:_setSmartCameraEnabled(false)
	
	local tileCenter = targetTile:getCenterVector()
	local newTranslate = self:_calculateTranslate(tileCenter)
	local newLookDir = tileCenter - newTranslate
	
	self.spanTranslate = SpanVariable.new(nil, newTranslate)
	self.spanLookDir = SpanVariable.new(nil, newLookDir)
	self:_waitCameraMove()
end

function CameraFacade:ascendAwayBattle()
	assert(self.spanTranslate, 'Illegal state to ascend away battle')
	
	self.spanTranslate:flip()
	self.spanLookDir:flip()
	
	self:_waitCameraMove()
	self:_setSmartCameraEnabled(true)
end

function CameraFacade:_lockedMove(speed, lookTarget)
	local cameraObject = self.cameraObject
	local cameraControl= self.cameraObject:findComponent(c'Camera')
	
	local targetVariable = {
		set = function (self, newLookTarget)
			local lookDir = cameraControl:getLookDirection()
			local translate = newLookTarget - lookDir
			cameraObject:resetTranslate(translate)
			cameraControl:setTarget(newLookTarget)
		end,
		
		get = function (self)
			return cameraControl:getTarget()
		end
	}
	
	local mod = ModifierFactory.vector(speed, lookTarget, targetVariable)
	mod.onPause = functional.bind2(self._setSmartCameraEnabled, self, true)
	mod.onResume= functional.bind2(self._setSmartCameraEnabled, self, false)
	return mod
end

function CameraFacade:translateToTarget(target)
	local targetModifier = self:_lockedMove(self.translateSpeed, target)
	local updater = ModifierAdapter.fit(self.cameraObject, targetModifier)

	AsConditionFactory.waitTargetModifier(targetModifier)

	targetModifier:setEnabled(false)
	updater:stop()
end

return CameraFacade