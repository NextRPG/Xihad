local exceptional = require 'std.exceptional'
local BattleManager = require 'GameFlow.BattleManager'
local PlayerController = {
	playerStateMachine = nil,
	controllingTeam= nil,
	endCallback = nil,
	checker = nil,
}
PlayerController.__index = PlayerController

function PlayerController.new(playerStateMachine)
	assert(playerStateMachine:getCurrentState() == 'Finish', 'Illegal state')
	
	local obj = setmetatable({
			playerStateMachine = playerStateMachine,
			controllingTeam= nil,
			checker = nil,
		}, PlayerController)
	playerStateMachine:addStateListener('Finish', obj)
	playerStateMachine:setBlocked(true)
	return obj
end

function PlayerController:isRunning()
	return self.controllingTeam ~= nil
end

function PlayerController:_check_running()
	assert(self:isRunning(), 'Illegal state')
end

function PlayerController:start(team, endCallback, checker)
	assert(team and not self:isRunning())
	self.playerStateMachine:setBlocked(false)
	
	self.controllingTeam= team
	self.endCallback = endCallback
	self.checker = checker
	
	self:_continue()
end

function PlayerController:_continue()
	if self.controllingTeam:hasActiveWarrior() then
		self.playerStateMachine:waitNextHero()
	else
		local endCallback = self.endCallback
		self:_release_control()
		endCallback:onRoundEnd()
	end
end

function PlayerController:_release_control()
	self:_check_running()
	
	self.playerStateMachine:setBlocked(true)
	self.controllingTeam= nil
	self.checker = nil
	self.endCallback = nil
end

function PlayerController:onStateEnter(state, prev)
	if prev == nil then return end
	
	self:_check_running()
	assert(state == 'Finish', state)
	
	if self.checker:onCheckPoint() == 'stop' then
		self:_release_control()
	else
		self:_continue()
	end
end

function PlayerController:onStateExit(state, next) 
	self:_check_running()
end

return PlayerController
