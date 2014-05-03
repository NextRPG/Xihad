local TimedAction = require 'TimedAction'
local CameraAction= {}

function CameraAction.lookDir(cameraControl, spanLookDir, duration, lerp)
	spanLookDir.origin = spanLookDir.origin or cameraControl:getLookDirection()
	
	local originQuaternion = math3d.quaternion()
	local finishQuaternion = math3d.quaternion(spanLookDir.origin, spanLookDir.finish)
	
	local action = TimedAction.new(duration, lerp)
	function action.onProgress(self, progress)
		assert(self == action)
		local q = math3d.lerp(originQuaternion, finishQuaternion, progress)
		cameraControl:setLookDirection(q * spanLookDir.origin)
	end
	
	return action
end

return CameraAction
