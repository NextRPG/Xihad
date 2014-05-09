local TimedAction = require 'Action.TimedAction'
local CameraAction= {}

function CameraAction.lookDir(cameraControl, spanLookDir, duration, lerp)
	spanLookDir.origin = spanLookDir.origin or cameraControl:getLookDirection()
	
	local finishLookDir = spanLookDir.finish:copy()
	local originQuaternion = math3d.quaternion()
	local finishQuaternion = math3d.quaternion(spanLookDir.origin, finishLookDir)
	
	local action = TimedAction.new(duration, lerp)
	function action.onProgress(self, progress)
		assert(self == action)
		local newLookDir
		if progress >= 1 then
			newLookDir = finishLookDir
		else
			local q = math3d.lerp(originQuaternion, finishQuaternion, progress)
			newLookDir = q * spanLookDir.origin
		end
		
		cameraControl:setLookDirection(newLookDir)
	end
	
	return action
end

return CameraAction
