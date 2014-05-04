local base = require 'Controller.PlayerState'
local ChooseCommandState = { }
ChooseCommandState.__index = ChooseCommandState
setmetatable(ChooseCommandState, base)

function ChooseCommandState.new(...)
	return setmetatable(base.new(...), ChooseCommandState)
end

function ChooseCommandState:onBack()
	-- TODO
	return 'back'
end

function ChooseCommandState:onTouch(x, y)
	-- ignore
end

function ChooseCommandState:onHover(x, y)
	-- ignore
end

function ChooseCommandState:onUICommand(command)
	if command == '待机' then
		local warrior = self.commandList.source
		self.executor:standBy(warrior)
		self.camera:focus(nil)
		return 'done'
	else
		self.commandList:setCommand(command)
		-- TODO
		-- self.chessboard:markAttackableRange(src, loc, cmd)
		return 'next'
	end
end

return ChooseCommandState
