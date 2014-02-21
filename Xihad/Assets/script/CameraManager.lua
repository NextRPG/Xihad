--- 
-- 管理相机的创建，移动和控制
-- @module CameraManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraManager = {
	
}

function CameraManager:createCamera( name )
	local camera = scene:createObject(c(name))
	camera.ccom = camera:appendComponent(c"Camera")
	camera.move = camera:appendComponent(c"ForeverMoveBy")
	camera.rotate = camera:appendComponent(c"RotateBy")
	camera.sequence = camera:appendComponent(c"Sequence")
	return camera
end

function CameraManager:init(  )
	
	local camera = self:createCamera("mainCamera")
	camera:concatTranslate(math3d.vector(40, 75, 35))
	camera.ccom:setUpVector(math3d.vector(0, 0, 1))
	camera.ccom:setTarget(math3d.vector(50, 0, 45))
	self.camera = camera
	-- scene:pushController(self)

end

function CameraManager:onMouseEvent( e )
	local camera = self.camera
	local ccom = camera:findComponent(c"Camera")
	local moveby = camera:findComponent(c"ForeverMoveBy")

	print(camera:getTranslate():xyz())

	if e.type == "mouseDragged" and e.deltaX ~= nil then

		moveby:runAction{x = e.deltaX, y = - e.deltaY} 

	elseif e.type == "mouseDraggedEnd" then

		moveby:stopAction()

	end
	-- print(e.wheelDelta)
	ccom:setFOV((1+0.05*e.wheelDelta) * ccom:getFOV())
end

return CameraManager