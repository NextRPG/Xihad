local Factory = {}

function Factory.createDefault(id, translate, target)
	translate = translate or math3d.vector(0, 25, 0)
	target = target or math3d.vector(35, 10, 25)
	
	local cameraObject = g_scene:createObject(c(id))
	if not cameraObject then
		error('Failed to create camera object with the specified name')
	end
	
	local cameraControl= cameraObject:appendComponent(c"Camera")
	cameraObject:resetTranslate(translate)
	cameraControl:setUpVector(math3d.vector(0, 1, 0))
	
	cameraControl:setTarget(target)
	
	return cameraObject
end

return Factory
