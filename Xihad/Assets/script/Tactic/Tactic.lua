local CommandList = require 'Command.CommandList'

local Tactic = {}
Tactic.__index = Tactic

function Tactic.new()
	return setmetatable({}, Tactic)
end

function Tactic:_createCommandList()
	return CommandList.new()
end

function Tactic:_completeOrder(cmdList)
	error('Tactic: No implementation by default')
end

---
-- @return CommandList
function Tactic:giveOrder()
	local cmdList = self:_createCommandList()
	
	cmdList:setSource(self:findPeer(c'Warrior'))
	
	self:_completeOrder(cmdList)
	
	return cmdList
end

return Tactic
