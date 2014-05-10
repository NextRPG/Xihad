local base = require 'Controller.PlayerState'

local ChooseCommandState = setmetatable({}, base)
ChooseCommandState.__index = ChooseCommandState

function ChooseCommandState.new(...)
	return setmetatable(base.new(...), ChooseCommandState)
end

function ChooseCommandState:onTouch(x, y)
	-- ignore
end

function ChooseCommandState:onHover(x, y)
	-- ignore
end

function ChooseCommandState:onUICommand(command)
	self.commandList:setCommand(command)
	
	if command == '待机' then
		self.executor:standBy(self.commandList:getSource())
		return 'done'
	end
	
	return 'next'
end

return ChooseCommandState
