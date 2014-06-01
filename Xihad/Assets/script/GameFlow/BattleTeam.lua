local Iterator = require 'std.Iterator'
local functional = require 'std.functional'
local BattleTeam = {
	teamName = nil,
	warriorController = nil,
}
BattleTeam.__index = BattleTeam

function BattleTeam.new(teamName, controller)
	return setmetatable({
			teamName = teamName,
			warriorController = controller,
		}, BattleTeam)
end

function BattleTeam:getTeamName()
	return self.teamName
end

function BattleTeam:allWarriors()
	error('no implementation by default')
end

function BattleTeam:belongToTeam(warrior)
	error('no implementation by default')
end

function BattleTeam:takeAction(endCallback, checker)
	for warrior in self:allWarriors() do
		warrior:activate()
	end
	
	if not checker:onCheckPoint() then
		self.warriorController:start(self, endCallback, checker)
	end
end

function BattleTeam:onBattleFinished()
end

function BattleTeam:allActiveWarriors()
	return Iterator.filter2(functional.asself('isActive'), self:allWarriors())
end

function BattleTeam:hasActiveWarrior()
	for _ in self:allActiveWarriors() do
		return true
	end
	
	return false
end

return BattleTeam
