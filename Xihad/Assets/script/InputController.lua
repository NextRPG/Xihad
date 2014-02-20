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
function judgeType( e )
	type = e.type
	if lastType == "mouseDragged" then
		local x, y = cursor:getPosition()
		e.deltaX = x - lastX
		e.deltaY = y - lastY
		print(e.deltaX, e.deltaY)
		lastX, lastY = x, y
		if type == "mouseMoved" then
			return "mouseDragged"
		elseif type == "lUplift" then
			lastType = ""
			return ""
		end
	elseif lastType == "lPressed" then
		lastX, lastY = cursor:getPosition()
		if type == "lUplift" then
			return "lClicked"
		elseif type == "mouseMoved" then
			return "mouseDragged"
		end
	elseif lastType == "lClicked" then
		lastType = ""
	end
	return type
end

-- private

function InputController:onMouseEvent( e )
	e.type = judgeType( e )
	if e.type == "wheelMoved" or e.type == "mouseDragged" then
		CameraManager:onMouseEvent( e )
	else 
		BattleManager:onMouseEvent( e )
	end
	lastType = e.type
	return true
end

function InputController:onKeyUp( e )
	BattleManager:onKeyUp(e)
	return true
end


return InputController