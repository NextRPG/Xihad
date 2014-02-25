--- 
-- 管理相机的创建，移动和控制
-- @module CameraManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraManager = {
	shift = math3d.vector(0, 64, -16),
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
	camera:concatTranslate(ccom:getTarget() + self.shift)
	self.state = "low"
	ccom:setUpVector(math3d.vector(0, 1, 0))
	self.camera = camera
	-- ccom:setFOV(0.85)
end

function CameraManager:onMouseEvent( e )
	local camera = self.camera
	local ccom = camera:findComponent(c"Camera")
	local forever = camera:findComponent(c"ForeverMoveBy")


	if e.type == "mouseDragged" and e.deltaX ~= nil then

		forever:runAction{x = - e.deltaX, y = e.deltaY} 

	elseif e.type == "mouseDraggedEnd" then

		forever:stopAction()

	end
	-- self:adjustHeight(e.wheelDelta)
	self.shift = self.shift * (1 + e.wheelDelta / 10)
	print(self.shift:xyz())


	-- camera:resetTranslate(ccom:getTarget() +  self.shift)
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

function CameraManager:adjustHeight( wheelDelta )
	local camera = self.camera
	local move = camera:findComponent(c"CameraMoveBy")
	local ccom = camera:findComponent(c"Camera")

	if self.state ~= "low" and wheelDelta > 0 then
		move:runAction{destination = ccom:getTarget() + self.shift}
		self.state = "low"
	elseif self.state ~= "high" and wheelDelta < 0 then
		move:runAction{destination = ccom:getTarget() + self.shift * 2}
		self.state = "high"
	end
end

function CameraManager:move2Tile( point )
	local camera = self.camera
	local move = camera:findComponent(c"CameraMoveBy")
	local action = {interval = 0.4}
	action.destination2 = point2vector(point)
	action.destination = action.destination2 + self.shift
	self.state = "low"	
	runAsyncFunc(move.runAction, move, action)
end

return CameraManager