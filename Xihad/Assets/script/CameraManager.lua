--- 
-- 管理相机的创建，移动和控制
-- @module CameraManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraManager = {
	
}

require "math3d"

function CameraManager:createCamera( name )
	local camera = scene:createObject(c(name))
	camera.ccom = camera:appendComponent(c"Camera")
	camera.move = camera:appendComponent(c"MoveBy")
	camera.rotate = camera:appendComponent(c"RotateBy")
	camera.sequence = camera:appendComponent(c"Sequence")
	return camera
end

function CameraManager:init(  )
	
	local camera = self:createCamera("mainCamera")
	camera:concatTranslate(50, 75, 45)
	camera.ccom:setUpVector(math3d.vector(0, 0, 1))
	camera.ccom:setTarget(math3d.vector(50, 0, 45))
	self.camera = camera
	-- scene:pushController(self)

end

function CameraManager:onMouseEvent( e )
	if e.type == "wheelMoved" then
		local ccom = self.camera:findComponent(c"Camera")
		print(e.wheelDelta)
		ccom:setFOV((1+0.05*e.wheelDelta) * ccom:getFOV())
	end
	return true
end

return CameraManager