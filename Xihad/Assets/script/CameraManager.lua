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
	camera:concatTranslate(40, 75, 35)
	camera.ccom:setUpVector(math3d.vector(0, 0, 1))
	camera.ccom:setTarget(math3d.vector(50, 0, 45))
	self.camera = camera
	-- scene:pushController(self)

end

function CameraManager:onMouseEvent( e )
	local camera = self.camera
	local ccom = camera:findComponent(c"Camera")

	if e.type == "mouseDragged" and e.deltaX ~= nil then
		print(camera:getTranslate())
		print(ccom:getTarget():xyz())
		camera:concatTranslate(e.deltaX, 0, - e.deltaY)
		local x, y, z = ccom:getTarget():xyz()
		ccom:setTarget(math3d.vector(x + e.deltaX, y, z - e.deltaY))
	end
	print(e.wheelDelta)
	ccom:setFOV((1+0.05*e.wheelDelta) * ccom:getFOV())
end

return CameraManager