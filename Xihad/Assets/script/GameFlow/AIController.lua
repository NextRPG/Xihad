local sCoroutine = require 'std.sCoroutine'
local functional = require 'std.functional'
local AIController = {
	commandExecutor = nil,
}
AIController.__index = AIController

function AIController.new(commandExecutor)
	return setmetatable({
			commandExecutor = commandExecutor,
		}, AIController)
end

function AIController:_run_warrior(warrior)
	local tactic = warrior:findPeer(c'Tactic')
	self.commandExecutor:execute(tactic:giveOrder())
end

function AIController:_run_until_no_active(team, checker)
	local hasActive = true
	while hasActive do
		hasActive = false
		for warrior in team:allActiveWarriors() do
			self:_run_warrior(warrior)
			if checker:onCheckPoint() then
				return true
			end
			
			hasActive = true
		end
	end
	
	return false
end

function AIController:_run_impl(team, endCallback, checker)
	if not self:_run_until_no_active(team, checker) then
		endCallback:onRoundEnd()
	end
end

function AIController:start(team, endCallback, checker)
	sCoroutine.start(
		functional.bindself(self, '_run_impl'), 
		team, endCallback, checker)
end

return AIController