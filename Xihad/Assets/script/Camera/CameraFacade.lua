local EaseFunction = require 'Ease.EaseFunction'
local SpanVariable = require 'Action.SpanVariable'
local ActionAdapter= require 'Action.ActionAdapter'
local SmoothAiming = require 'Camera.SmoothAiming'
local CameraAvoid  = require 'Camera.CameraAvoid'
local ThirdPersonFollow = require 'Camera.ThirdPersonFollow'
local AsConditionFactory= require 'Action.AsConditionFactory'
local CameraMovement	= require 'HighAction.CameraMovement'
local CameraFacade = {
	cameraObject = nil,
	followControl = nil,
	aimingControl = nil,
	cameraAvoid   = nil,
	
	focusedObject = nil,
	spanTranslate = nil,
	spanLookDir   = nil,
}
CameraFacade.__index = CameraFacade

function CameraFacade.new(cameraObject)
	local o = setmetatable({
			cameraObject = cameraObject,
			aimingControl = SmoothAiming.new(cameraObject, math3d.vector(0.5, -0.5, 0.5)),
			followControl = ThirdPersonFollow.new(cameraObject),
			cameraAvoid   = CameraAvoid.new(cameraObject),
		}, CameraFacade)
	
	ActionAdapter.fit(cameraObject, o.cameraAvoid)
	ActionAdapter.fit(cameraObject, o.followControl)
	ActionAdapter.fit(cameraObject, o.aimingControl)
	return o
end

function CameraFacade:focus(object)
	self.focusedObject = object
	
	self.aimingControl:setAim(object)
	self.followControl:setFollowing(object)
	
	AsConditionFactory.waitCameraAim(self.aimingControl)
	if object then
		if self.followControl._state == 'focusing' then
			AsConditionFactory.waitCameraFocus(self.followControl)
		end
	end
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
	
	local x, y, z = 8, 20, 10
	local offset = xDirection * x + zDirection * z + yDirection * y
	
	return oldTarget + offset
end

function CameraFacade:_waitCameraMove()
	local action = CameraMovement.freelyMove(
		self.cameraObject, self.spanTranslate, self.spanLookDir, 20, math.sqrt)
	
	ActionAdapter.fit(self.cameraObject, action)
	AsConditionFactory.waitAction(action)
end

function CameraFacade:descendIntoBattle(targetTile)
	if not self.focusedObject then
		error('Illegal state to descend into battle')
	end
	
	self:_setSmartCameraEnabled(false)
	
	local newTranslate = self:_calculateTranslate(targetTile:getCenterVector())
	
	local newTarget = targetTile:getCenterVector() + math3d.vector(0, 5, 0)
	local newLookDir= newTarget - newTranslate
	
	self.spanTranslate = SpanVariable.new(nil, newTranslate)
	self.spanLookDir = SpanVariable.new(nil, newLookDir)
	
	self:_waitCameraMove()
end

function CameraFacade:ascendAwayBattle()
	if not self.spanTranslate then
		error('Illegal state to ascend away battle')
	end
	
	self.spanTranslate:flip()
	self.spanLookDir:flip()
	
	self:_waitCameraMove()
	self:_setSmartCameraEnabled(true)
end

return CameraFacade