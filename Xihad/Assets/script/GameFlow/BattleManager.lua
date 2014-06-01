local Class = require 'std.Class'
local ExclusiveChecker = require 'Condition.ExclusiveChecker'
local BattleManager = {
	teams  = nil,
	roundCounter = 0,
	victoryChecker = nil,
	actionTeamIndex= nil,
}
BattleManager.__index = BattleManager

function BattleManager.new()
	local m = setmetatable({
			teams  = {},
		}, BattleManager)
	
	local victoryListener = {}
	Class.delegateClosure(victoryListener, 'onChecked', m, 'onVictory')
	m.victoryChecker = ExclusiveChecker.new(victoryListener)
	
	-- local scriptListener = {}
	-- Class.delegateClosure(scriptListener, 'onChecked', m, 'executeScript')
	-- m.scriptChecker = BaseChecker.new(scriptListener)
	
	return m
end

Class.delegate(BattleManager, 'addVictoryCondition', 
				'victoryChecker', 'addCondition')

Class.delegate(BattleManager, 'removeVictoryCondition', 
				'victoryChecker', 'removeCondition')

function BattleManager:onTeamWon(winTeam)
	print('Team won:', winTeam:getTeamName())
end

function BattleManager:onVictory(teamName)
	local team = self:findTeam(teamName)
	if not team then
		return false
	end
	
	self:onTeamWon(team)
	return true
end

function BattleManager:getCurrentRound()
	return self.roundCounter
end

function BattleManager:addTeam(team)
	local teamName = team:getTeamName()
	if self:findTeam(teamName) ~= nil then
		error('duplicated team name')
	end
	
	table.insert(self.teams, team)
	return team
end

function BattleManager:findTeam(teamName)
	for _, team in ipairs(self.teams) do
		if team:getTeamName() == teamName then
			return team
		end
	end
end

function BattleManager:getActionTeam()
	return self.teams[self.actionTeamIndex]
end

function BattleManager:_nextTeam()
	local prevTeamIndex, nextTeam = self.actionTeamIndex
	self.actionTeamIndex, nextTeam = next(self.teams, prevTeamIndex)
	
	if nextTeam then
		nextTeam:takeAction(self, self.victoryChecker)
	elseif prevTeamIndex then
		self:nextRound()
	else -- no prev team and no next team
		error('no team added')
	end
end

function BattleManager:onRoundEnd()
	self:_nextTeam()
end

function BattleManager:nextRound()
	self.roundCounter = self.roundCounter + 1
	if self.victoryChecker:onCheckPoint() then
		self:_nextTeam()
	end
end

function BattleManager:startBattle()
	-- init battle?
	self:nextRound()
end

return BattleManager