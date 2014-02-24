--- 
-- 管理相机的创建，移动和控制
-- @module CameraManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraManager = {
	shift = math3d.vector(-50, 50, -50)
}

function CameraManager:createCamera( name )
	local camera = scene:createObject(c(name))
	camera:appendComponent(c"Camera")
	camera:appendComponent(c"ForeverMoveBy")
	camera:appendComponent(c"CameraMoveBy")
	return camera
end

function CameraManager:init(  )
	
	local camera = self:createCamera("mainCamera")
	local ccom = camera:findComponent(c"Camera")
	ccom:setTarget(math3d.vector(Consts.COLS * Consts.TILE_WIDTH / 2, 0, Consts.ROWS * Consts.TILE_HEIGHT / 2))
	camera:concatTranslate(math3d.vector(0, 50, -5))
	ccom:setUpVector(math3d.vector(0, 1, 0))
	self.camera = camera
	ccom:setFOV(0.85)
	-- scene:pushController(self)

end

function CameraManager:onMouseEvent( e )
	local camera = self.camera
	local ccom = camera:findComponent(c"Camera")
	local forever = camera:findComponent(c"ForeverMoveBy")

	print(camera:getTranslate():xyz())

	if e.type == "mouseDragged" and e.deltaX ~= nil then

		forever:runAction{x = e.deltaX, y = - e.deltaY} 

	elseif e.type == "mouseDraggedEnd" then

		forever:stopAction()

	end
	-- ccom:setFOV((1+0.05*e.wheelDelta) * ccom:getFOV())
	-- print(ccom:getFOV())
end

local backAction = {}
function CameraManager:onKeyUp( e )
	local camera = self.camera
	local move = camera:findComponent(c"CameraMoveBy")
	local ccom = camera:findComponent(c"Camera")

	if e.key == "UP" then
		backAction = {destination = self.camera:getTranslate(), destination2 = ccom:getTarget()}
		move:moveToCharacter(scene:findObject(c"1"))
	elseif e.key == "DOWN" then

		move:runAction(backAction)
	end
end

return CameraManager