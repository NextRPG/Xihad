--- 
-- 管理相机的创建，移动和控制
-- @module CameraManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraManager = {
	shift = math3d.vector(0, 48, -16) * 1.5,
}

function CameraManager:createCamera( name )
	local camera = g_scene:createObject(c(name))
	camera:appendComponent(c"Camera")
	camera:appendComponent(c"ForeverMoveBy")
	camera:appendComponent(c"CameraMoveBy")
	camera:appendComponent(c"CameraFollow")
	camera:appendComponent(c"CameraRotate")
	return camera
end

function CameraManager:init(  )
	local camera = self:createCamera("mainCamera")
	local ccom = camera:findComponent(c"Camera")
	-- ccom:setTarget(math3d.vector(Consts.COLS * Consts.TILE_WIDTH / 2, 0, Consts.ROWS * Consts.TILE_HEIGHT / 2))
	-- camera:concatTranslate(ccom:getTarget() + self.shift)
	self.state = "low"
	-- ccom:setUpVector(math3d.vector(0, 0, 1))
	ccom:setUpVector(math3d.vector(0, 1, 0))

	self.camera = camera
	ccom:setFOV(0.85)
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
	self:adjustHeight(e.wheelDelta)
end

-- local backAction = {}
function CameraManager:onKeyUp( e )
-- 	local camera = self.camera
-- 	local move = camera:findComponent(c"CameraMoveBy")
-- 	local ccom = camera:findComponent(c"Camera")
-- 	local rotate = camera:findComponent(c"CameraRotate")


-- 	local HeroManager = require "HeroManager"
-- 	if e.key == "UP" then
-- 		backAction = {destination = self.camera:getTranslate(), destination2 = ccom:getTarget()}
-- 		move:moveToCharacter(HeroManager.currentCharacter, function (  )
-- 			rotate:start(math3d.vector(0, 1, 0))
-- 		end)
-- 	elseif e.key == "DOWN" then
-- 		rotate:stop()
-- 		move:runAction(backAction)
-- 	-- elseif e.key == "Z" then
-- 	-- 	ccom:setUpVector(ccom:getUpVector() + math3d.vector(1, 0, 0))
-- 	-- elseif e.key == "X" then
-- 	-- 	ccom:setUpVector(ccom:getUpVector() + math3d.vector(0, 1, 0))
-- 	-- elseif e.key == "C" then
-- 	-- 	ccom:setUpVector(ccom:getUpVector() + math3d.vector(0, 0, 1))
-- 	end
-- 	-- print(ccom:getUpVector():xyz())
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

function CameraManager:move2vector( vector )
	local camera = self.camera
	local move = camera:findComponent(c"CameraMoveBy")
	local action = {}
	action.destination2 = vector
	action.destination = action.destination2 + self.shift
	self.state = "low"
	runAsyncFunc(move.runActionByDelta, move, action)
end

function CameraManager:move2Character( characterObject )
	self:move2vector( characterObject:getTranslate() )
end

function CameraManager:move2Battle( characterObject, tile )
	local camera = self.camera
	local move = camera:findComponent(c"CameraMoveBy")
	local ccom = camera:findComponent(c"Camera")
	local action = {}
	action.destination2 = (characterObject:getTranslate() + point2vector(tile)) * 0.5 + math3d.vector(0, 20 ,0)
	local x1, y1 = (point2vector(tile) - characterObject:getTranslate()):xyz()
	local x2 = math.sqrt(900 / (1 + (x1 * x1) / (y1 * y1)))
	local y2 = math.sqrt(900 - x2 * x2)
	action.destination = action.destination2 + math3d.vector( - x2, 10, - y2)
	backAction = {destination = self.camera:getTranslate(), destination2 = ccom:getTarget()}
	runAsyncFunc(move.runAction, move, action)
end

function CameraManager:back2Normal(  )
	local camera = self.camera
	local move = camera:findComponent(c"CameraMoveBy")
	runAsyncFunc(move.runAction, move, backAction)
end

return CameraManager