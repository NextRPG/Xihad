local ActionAdapter = require 'ActionAdapter'
local ActionFactory = require 'ActionFactory'
local ObjectAction  = require 'ObjectAction'
local CameraAction  = require 'CameraAction'
local camMovement = {}

-- 锁定 lookDirection 后移动
function camMovement.lockedMove(cameraObject, spanTranslate, duration, lerp)
	local cameraControl = cameraObject:findComponent(c'Camera')
	cameraControl:setTargetFixed(false)
	
	local action = ObjectAction.move(cameraObject, spanTranslate, duration, lerp)
	ActionAdapter.fit(cameraObject, action)
	return action
end

function camMovement.freelyMove(cameraObject, spanTranslate, spanLookDir, duration, lerp)
	local moveAction = ObjectAction.move(cameraObject, spanTranslate, duration, lerp)
	
	local cameraControl = cameraObject:findComponent(c'Camera')
	local lookDirAction = CameraAction.lookDir(cameraControl, spanLookDir, duration, lerp)
	
	local parallel = ActionFactory.parallel( { moveAction, lookDirAction } )
	ActionAdapter.fit(cameraObject, parallel)
	return parallel
end

return camMovement
