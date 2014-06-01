local base = require 'Controller.PlayerState'
local FinishState = setmetatable({}, base)
FinishState.__index = FinishState

function FinishState.new(...)
	return setmetatable(base.new(...), FinishState)
end

function FinishState:needTouch()
	return false
end

function FinishState:needHover()
	return false
end

function FinishState:onBack()
end

function FinishState:onUICommand(command, subcommand)
end

return FinishState