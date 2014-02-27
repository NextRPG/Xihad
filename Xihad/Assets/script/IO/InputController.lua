--- 
-- 处理键盘和鼠标的输入时间
-- @module InputController
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local InputController = {
}

local CameraManager = require "CameraManager"
local BattleManager = require "BattleManager"

-- private
local lastType = ""
local lastX, lastY = 0, 0
local lastTime = 0
local function judgeType( e )
	local type = e.type
	if lastType == "mouseDragged" then
		local x, y = cursor:getPosition()
		e.deltaX = x - lastX
		e.deltaY = y - lastY
		lastX, lastY = x, y
		e.interval = Time.change - lastTime
		lastTime = Time.change
		if type == "mouseMoved" then
			return "mouseDragged"
		elseif type == "lUplift" then
			lastType = ""
			return "mouseDraggedEnd"
		end
	elseif lastType == "lPressed" then
		lastX, lastY = cursor:getPosition()
		lastTime = Time.change
		if type == "lUplift" then
			return "lClicked"
		elseif type == "mouseMoved" then
			return "mouseDragged"
		end
	elseif lastType == "lClicked" or "mouseDraggedEnd" then
		lastType = ""
	end
	return type
end

-- private

function InputController:onMouseEvent( e )
	e.type = judgeType( e )
	if e.type == "wheelMoved" or e.type == "mouseDragged" or e.type == "mouseDraggedEnd" then
		CameraManager:onMouseEvent( e )
	else 
		BattleManager:onMouseEvent( e )
	end
	lastType = e.type
	return true
end

function InputController:onKeyUp( e )
	CameraManager:onKeyUp(e)
	BattleManager:onKeyUp(e)
	return true
end


return InputController