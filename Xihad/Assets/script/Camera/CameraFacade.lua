local ActionAdapter= require 'Action.ActionAdapter'
local SmoothAiming = require 'Camera.SmoothAiming'
local CameraAvoid  = require 'Camera.CameraAvoid'
local ThirdPersonFollow = require 'Camera.ThirdPersonFollow'
local AsConditionFactory= require 'Action.AsConditionFactory'
local CameraFacade = {
	cameraObject = nil,
	followControl = nil,
	aimingControl = nil,
	cameraAvoid   = nil,
}
CameraFacade.__index = CameraFacade

function CameraFacade.new(cameraObject)
	local o = setmetatable({
			cameraObject = cameraObject,
			aimingControl = SmoothAiming.new(cameraObject, math3d.vector(0.5, -0.5, 0.5)),
			followControl = ThirdPersonFollow.new(cameraObject),
			cameraAvoid   = CameraAvoid.new(cameraObject),
		}, CameraFacade)
	
	ActionAdapter.fit(cameraObject, o.aimingControl)
	ActionAdapter.fit(cameraObject, o.cameraAvoid)
	ActionAdapter.fit(cameraObject, o.followControl)
	return o
end

local XihadMapTile = require 'Chessboard.XihadMapTile'
function CameraFacade:focus(object)
	self.aimingControl:setAim(object)
	AsConditionFactory.waitCameraAim(self.aimingControl)
	
	self.followControl:setFollowing(object)
	if object then
		AsConditionFactory.waitCameraFocus(self.followControl)
	end
end

function CameraFacade:descendIntoBattle()
	-- TODO
end

function CameraFacade:ascendAwayBattle()
	-- TODO
end

return CameraFacade