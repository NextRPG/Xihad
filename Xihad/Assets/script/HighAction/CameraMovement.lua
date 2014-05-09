local ActionAdapter = require 'Action.ActionAdapter'
local ActionFactory = require 'Action.ActionFactory'
local ObjectAction  = require 'HighAction.ObjectAction'
local CameraAction  = require 'HighAction.CameraAction'
local CameraMovement = {}

-- 锁定 lookDirection 后移动
function CameraMovement.lockedMove(cameraObject, spanTranslate, duration, lerp)
	local cameraControl = cameraObject:findComponent(c'Camera')
	cameraControl:setTargetFixed(false)
	
	local action = ObjectAction.move(cameraObject, spanTranslate, duration, lerp)
	ActionAdapter.fit(cameraObject, action)
	return action
end

function CameraMovement.freelyMove(cameraObject, spanTranslate, spanLookDir, speed, lerp)
	local moveAction = ObjectAction.move(cameraObject, spanTranslate, speed, lerp)
	
	local duration = moveAction.duration
	local cameraControl = cameraObject:findComponent(c'Camera')
	local lookDirAction = CameraAction.lookDir(cameraControl, spanLookDir, duration, lerp)
	
	local parallel = ActionFactory.parallel( { moveAction, lookDirAction } )
	ActionAdapter.fit(cameraObject, parallel)
	return parallel
end

return CameraMovement
