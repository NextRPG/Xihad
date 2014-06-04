local Vector = require 'math.Vector'
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
	battleOffset= math3d.vector(8, 18, 15),
	tradeOffset = math3d.vector(15, 15, 0),
	battleHeightRatio= 0,
	tradeHeightRatio = -0.1,
	
	translateSpeed = 90, 
	slideSpeed = 45,
	slideEase  = EaseFunction.wrapInOut(EaseFunction[3]),
	
	cameraObject  = nil,
	followControl = nil,
	aimingControl = nil,
	cameraAvoid   = nil,
	
	focusedObject = nil,
	spanTranslate = nil,
	spanLookDir   = nil,
	
	autoResetListener = nil,
}
CameraFacade.__index = CameraFacade

function CameraFacade.new(cameraObject)
	local cameraControl = cameraObject:findComponent(c'Camera')
	local following = {
		getTranslate = functional.bindself(cameraControl, 'getTarget'),
	}
	
	local o = setmetatable({
			cameraObject  = cameraObject,
			aimingControl = SmoothAiming.new(cameraObject),
			followControl = DirectFollow.new(cameraObject),
			cameraAvoid   = DirectAvoid.new(cameraObject, following),
		}, CameraFacade)
	
	o.followControl:setFollowing(following)
	function o.autoResetListener(warrior, pname, prev)
		if warrior:isDead() then
			o:focus(nil)
		end
	end
	
	ModifierAdapter.fit(cameraObject, o.cameraAvoid)
	ModifierAdapter.fit(cameraObject, o.followControl)
	ModifierAdapter.fit(cameraObject, o.aimingControl)
	
	return o
end

function CameraFacade:_listenObject(object, add)
	if not object then return end
	
	local warrior = object:findComponent(c'Warrior')
	if not warrior then return end
	
	if add then
		warrior:addPropertyListener('Dead', self.autoResetListener)
	else
		warrior:removePropertyListener('Dead', self.autoResetListener)
	end
end

function CameraFacade:focus(object)
	if self.focusedObject ~= object then
		self:_listenObject(self.focusedObject, false)
		
		self.focusedObject = object
		self.aimingControl:setAim(object)
		AsConditionFactory.waitCameraAim(self.aimingControl)
		
		self:_listenObject(object, true)
	end
end

function CameraFacade:_setSmartCameraEnabled(b)
	self.cameraAvoid:setEnabled(b)
	self.followControl:setEnabled(b)
	self.aimingControl:setEnabled(b)
end

function CameraFacade:_calculateTranslateZ(zDirection, offsetSourceVec)
	local yDirection = math3d.vector(0, 1, 0)
	local oldTranslate = self.focusedObject:getTranslate()
	
	zDirection:set(nil, 0, nil)
	zDirection:normalize()
	
	local xDirection= zDirection:cross(yDirection)
	xDirection:normalize()
	
	local cameraPosition = self.cameraObject:getTranslate()
	if xDirection:dot(cameraPosition - oldTranslate) < 0 then
		xDirection = -xDirection
	end
	
	local x, y, z = offsetSourceVec:xyz()
	return oldTranslate + xDirection*x + yDirection*y + zDirection*z
end

function CameraFacade:_calculateTranslate(newTarget, offsetSourceVec)
	return self:_calculateTranslateZ(
			self.focusedObject:getTranslate() - newTarget, offsetSourceVec)
end

function CameraFacade:_waitCameraMove()
	self:_setSmartCameraEnabled(false)
	local action = CameraMovement.slide(self.cameraObject, 
		self.spanTranslate, self.spanLookDir, self.slideSpeed, self.slideEase)
	
	ActionAdapter.fit(self.cameraObject, action)
	AsConditionFactory.waitAction(action)
end

function CameraFacade:_descend(newTranslate, targetPos, ratio)
	local newLookDir = targetPos - newTranslate
	newLookDir:set(nil, Vector.getY(newLookDir)*ratio, nil)
	
	self.spanTranslate = SpanVariable.new(nil, newTranslate)
	self.spanLookDir = SpanVariable.new(nil, newLookDir)
	
	self:_waitCameraMove()
end

function CameraFacade:descendIntoBattle(targetTile)
	assert(self.focusedObject, 'Illegal state to descend into battle')
	local targetPos = targetTile:getCenterVector()
	local newTranslate = self:_calculateTranslate(targetPos, self.battleOffset)
	
	self:_descend(newTranslate, targetPos, self.battleHeightRatio)
end

function CameraFacade:descendIntoTransaction(guestObject)
	assert(self.focusedObject, 'Illegal state to descend into battle')
	
	local guestPos = guestObject:getTranslate()
	local masterPos= self.focusedObject:getTranslate()
	local zDirection = masterPos - guestPos
	local offsetSourceVec = self.tradeOffset:copy()
	offsetSourceVec:set(nil, nil, -(zDirection:length() / 2))
	
	local newTranslate = self:_calculateTranslateZ(zDirection, offsetSourceVec)
	local targetPos = (masterPos + guestPos) / 2
	self:_descend(newTranslate, targetPos, self.tradeHeightRatio)
end

function CameraFacade:ascendBack()
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