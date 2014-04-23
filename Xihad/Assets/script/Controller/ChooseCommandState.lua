local base = require 'Controller.PlayerControlState'
local ChooseCommandState = { }
ChooseCommandState.__index = ChooseCommandState
setmetatable(ChooseCommandState, base)

function ChooseCommandState.new()
	local obj = setmetatable(base.new(), ChooseCommandState)
	return obj
end

function ChooseCommandState:onBack()
	return 'back'
end

function ChooseCommandState:onTouch(x, y)
	-- ignore
end

function ChooseCommandState:onHover(x, y)
	-- ignore
end

function ChooseCommandState:onUICommand(command)
	if command == 'Standby' then
		local src = self.command:getSource()
		src:deactivate()
		return 'done'
	else
		self.commandList:setCommand(command)
		-- TODO
		-- self.chessboard:markAttackableRange(src, loc, cmd)
		return 'next'
	end
end

return ChooseCommandState
