local StateMachine = require 'std.StateMachine'
local ChooseHeroState = require 'Controller.ChooseHeroState'
local ChooseTileState = require 'Controller.ChooseTileState'
local ChooseTargetState = require 'Controller.ChooseTargetState'
local ChooseCommandState = require 'Controller.ChooseCommandState'
local Class = require 'std.Class'

local PlayController = {
	sm = nil,
	stateControllers = nil,
}
PlayController.__index = PlayController

function PlayController.new()
	local obj = setmetatable({
			sm = StateMachine.new('ChooseHero'),
			stateControllers = {},
		}, PlayController)
	
	obj.sm:setTransition('ChooseHero', 'next', 'ChooseTile')
	
	obj.sm:setTransition('ChooseTile', 'next', 'ChooseCommand')
	obj.sm:setTransition('ChooseTile', 'back', 'ChooseHero')
	obj.sm:setTransition('ChooseTile', 'fail', 'ChooseHero')
	
	obj.sm:setTransition('ChooseCommand', 'next', 'ChooseTarget')
	obj.sm:setTransition('ChooseCommand', 'back', 'ChooseTile')
	obj.sm:setTransition('ChooseCommand', 'fail', 'ChooseHero')
	
	obj.sm:setTransition('ChooseTarget', 'next', 'ChooseHero')
	obj.sm:setTransition('ChooseTarget', 'back', 'ChooseCommand')
	obj.sm:setTransition('ChooseTarget', 'fail', 'ChooseHero')
	
	obj.stateControllers['ChooseHero'] = ChooseHeroState.new()
	obj.stateControllers['ChooseTile'] = ChooseTileState.new()
	obj.stateControllers['ChooseCommand']= ChooseCommandState.new()
	obj.stateControllers['ChooseTarget'] = ChooseTargetState.new()
	
	return obj
end

Class.delegate(PlayController, 'addStateListener', 	 'sm')
Class.delegate(PlayController, 'removeStateListener','sm')
Class.delegate(PlayController, 'isInState', 		 'sm')
Class.delegate(PlayController, 'getCurrentState',    'sm')

function PlayController:_getCurrentStateController()
	return self.stateControllers[self:getCurrentState()]
end

function PlayController:_changeState(msg)
	if msg then
		self.sm:onMessage(msg)
	end
end

function PlayController:onHover(x, y)
	local msg = self:_getCurrentStateController():onHover(x, y)
	self:_changeState(msg)
end

function PlayController:onTouch(x, y)
	local msg = self:_getCurrentStateController():onTouch(x, y)
	self:_changeState(msg)
end

function PlayController:onBack()
	local msg = self:_getCurrentStateController():onBack()
	self:_changeState(msg)
end

function PlayController:onUICommand(cmd)
	local msg = self:_getCurrentStateController():onUICommand(cmd)
	self:_changeState(msg)
end

if select('#', ...) == 0 then 
	local instance = PlayController.new()
	assert(instance:getCurrentState() == instance.sm:getCurrentState())
end

return PlayController

