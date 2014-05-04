local Class = require 'std.Class'
local ActionAdapter= require 'Action.ActionAdapter'
local SmoothAiming = require 'Camera.SmoothAiming'
local AsConditionFactory= require 'Action.AsConditionFactory'
local ThirdPersonFollow = require 'Camera.ThirdPersonFollow'
local CameraFacade = {
	cameraObject = nil,
	followControl = nil,
	aimingControl = nil,
}
CameraFacade.__index = CameraFacade

function CameraFacade.new(cameraObject)
	local o = setmetatable({
			cameraObject = cameraObject,
			aimingControl = SmoothAiming.new(cameraObject, math3d.vector(0.8, -0.5, 1)),
			followControl = ThirdPersonFollow.new(cameraObject),
		}, CameraFacade)
	
	ActionAdapter.fit(cameraObject, o.followControl)
	ActionAdapter.fit(cameraObject, o.aimingControl)
	return o
end

function CameraFacade:focus(object)
	self.aimingControl:setAim(object)
	self.followControl:setFollowing(object)
	
	if object then
		AsConditionFactory.waitCameraFocus(self.followControl)
	end
end

-- function CameraFacade:...()
-- end

return CameraFacade