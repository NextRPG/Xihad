local Class = require 'std.Class'
local sCoroutine = require 'std.sCoroutine'
local functional = require 'std.functional'
local BaseChecker= require 'Condition.BaseChecker'
local ExclusiveChecker= require 'Condition.ExclusiveChecker'
local BattleManager = {
	teams  = nil,
	roundCounter = 0,
	victoryChecker = nil,
	commandChecker = nil,
	actionTeamIndex= nil,
	winTeam = nil,
}
BattleManager.__index = BattleManager

local commandCheckerListener = {}
function commandCheckerListener:onChecked(command)
	command()
end

function BattleManager.new()
	local m = setmetatable({
			teams  = {},
		}, BattleManager)
	
	local victoryListener = {}
	Class.delegateClosure(victoryListener, 'onChecked', m, 'onTeamWon')
	m.victoryChecker = ExclusiveChecker.new(victoryListener)
	
	m.commandChecker = BaseChecker.new(commandCheckerListener)
	
	return m
end

function BattleManager:addVictoryCondition(condition, teamName)
	local team = self:findTeam(teamName)
	assert(team ~= nil)
	self.victoryChecker:addCondition(condition, team)
end

Class.delegate(BattleManager, 'removeVictoryCondition', 
				'victoryChecker', 'removeCondition')

function BattleManager:onTeamWon(winTeam)
	print('Team won:', winTeam:getTeamName())
	self.winTeam = winTeam
end

function BattleManager:addCommandCondition(condition, command)
	assert(type(command) == 'function')
	self.commandChecker:addCondition(condition, command)
end

Class.delegate(BattleManager, 'removeCommandCondition',
				'commandChecker', 'removeCondition')

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

local function _check_coroutine_yieldable()
	assert(coroutine.running() ~= nil)
end

function BattleManager:onCheckPoint()
	_check_coroutine_yieldable()
	
	self.commandChecker:onCheckPoint()
	self.victoryChecker:onCheckPoint()
	if self.winTeam ~= nil then
		return 'stop'
	end
end

function BattleManager:_nextTeam() 	-- no throw
	local prevTeamIndex, nextTeam = self.actionTeamIndex
	self.actionTeamIndex, nextTeam = next(self.teams, prevTeamIndex)
	
	if nextTeam then
		nextTeam:takeAction(self, self)
	elseif prevTeamIndex then
		self:_nextRound()
	else -- no prev team and no next team
		error('no team added')
	end
end

function BattleManager:_nextRound() 	-- no throw
	self.roundCounter = self.roundCounter + 1
	if self:onCheckPoint() ~= 'stop' then
		self:_nextTeam()
	end
end

function BattleManager:onRoundEnd()	-- no throw
	sCoroutine.start(self._nextTeam, self)
end

function BattleManager:startBattle()
	-- init battle?
	sCoroutine.start(self._nextRound, self)
end

return BattleManager