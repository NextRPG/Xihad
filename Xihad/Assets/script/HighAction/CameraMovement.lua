local Vector = require 'math.Vector'
local Trigonometry  = require 'math.Trigonometry'
local TimedAction	= require 'Action.TimedAction'
local CameraMovement = {}

function CameraMovement.slide(cameraObject, spanTranslate, spanLookDir, speed, lerp)
	local cameraControl = cameraObject:findComponent(c'Camera')
	
	if not spanTranslate.origin then
		spanTranslate.origin = cameraObject:getTranslate()
	end
	
	if not spanLookDir.origin then
		spanLookDir.origin = cameraControl:getLookDirection()
	end
	
	local spanDistance = spanLookDir:transform(function (lookDir)
		local x, _, z = lookDir:xyz()
		return math.sqrt(x*x + z*z)
	end)
	local spanTarget = spanTranslate + spanLookDir
	local spanHeight = spanTranslate:transform(Vector.getY)
	local spanAngle  = spanLookDir:transform(function (lookDir)
		local x, _, z = lookDir:xyz()
		return math.atan2(z, x)	-- z is logic y-axis, x is logic x-axis
	end)
	
	spanAngle.finish = Trigonometry.toRadian(Trigonometry.nearestTarget(
			Trigonometry.toDegree(spanAngle.origin), 
			Trigonometry.toDegree(spanAngle.finish)))
	
	local duration = spanTranslate:delta():length() / speed
	local action = TimedAction.new(duration, lerp)
	function action.onProgress(self, d)
		local angle = spanAngle:interpolate(d)
		local lookDir = math3d.vector(math.cos(angle), 0, math.sin(angle))
		lookDir:setLength(spanDistance:interpolate(d))
		
		local target = spanTarget:interpolate(d)
		local translate = target - lookDir
		translate:set(nil, spanHeight:interpolate(d))
		
		cameraObject:resetTranslate(translate)
		cameraControl:setTarget(target)
	end
	
	return action
end

return CameraMovement
