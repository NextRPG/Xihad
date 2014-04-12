local camAction = {}

function camAction.lookDir(cameraControl, orginLookDir, finishLookDir, duration, lerp)
	originLookDir = originLookDir or cameraControl:getLookDirection()
	local originQuaternion = math3d.quaternion()
	local finishQuaternion = math3d.quaternion(originLookDir, finishLookDir)
	
	local action = TimedAction.new(duration, lerp)
	function action.onProgress(self, progress)
		assert(self == action)
		local q = math3d.lerp(originQuaternion, finishQuaternion, progress)
		cameraControl:setLookDir(q * originLookDir)
	end
	
	return action
end
