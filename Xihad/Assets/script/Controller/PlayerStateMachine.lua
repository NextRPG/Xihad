local CommandList = require 'Command.CommandList'
local StateMachine = require 'std.StateMachine'
local ChooseHeroState = require 'Controller.ChooseHeroState'
local ChooseTileState = require 'Controller.ChooseTileState'
local ChooseTargetState = require 'Controller.ChooseTargetState'
local ChooseCommandState = require 'Controller.ChooseCommandState'
local CursorEventDispatcher = require 'Controller.CursorEventDispatcher'
local Class = require 'std.Class'

local PlayStateMachine = {
	sm = nil,
	runner = nil,
	stateControllers = nil,
}
PlayStateMachine.__index = PlayStateMachine

function PlayStateMachine.new(ui, camera, painter, executor)
	local obj = setmetatable({
			sm = StateMachine.new('ChooseHero'),
			stateControllers = {},
		}, PlayStateMachine)
	
	obj.sm:setTransition('ChooseHero', 'next', 'ChooseTile')
	
	obj.sm:setTransition('ChooseTile', 'next', 'ChooseCommand')
	obj.sm:setTransition('ChooseTile', 'back', 'ChooseHero')
	
	obj.sm:setTransition('ChooseCommand', 'next', 'ChooseTarget')
	obj.sm:setTransition('ChooseCommand', 'done', 'Finish')
	obj.sm:setTransition('ChooseCommand', 'back', 'ChooseTile')
	
	obj.sm:setTransition('ChooseTarget', 'next', 'Finish')
	obj.sm:setTransition('ChooseTarget', 'back', 'ChooseCommand')
	
	---
	-- @see nextHero()
	obj.sm:setTransition('Finish', 'continue', 'ChooseHero')
	
	local commandList = CommandList.new()
	local newDispatcher  = CursorEventDispatcher.new
	obj.stateControllers['ChooseHero'] = ChooseHeroState.new(commandList, newDispatcher, ui, camera, painter, executor)
	obj.stateControllers['ChooseTile'] = ChooseTileState.new(commandList, newDispatcher, ui, camera, painter, executor)
	obj.stateControllers['ChooseCommand']= ChooseCommandState.new(commandList, newDispatcher, ui, camera, painter, executor)
	obj.stateControllers['ChooseTarget'] = ChooseTargetState.new(commandList, newDispatcher, ui, camera, painter, executor)
	
	return obj
end

Class.delegate(PlayStateMachine, 'addStateListener',	'sm')
Class.delegate(PlayStateMachine, 'removeStateListener',	'sm')
Class.delegate(PlayStateMachine, 'isInState',			'sm')
Class.delegate(PlayStateMachine, 'getCurrentState',		'sm')

function PlayStateMachine:_getCurrentStateController()
	return self.stateControllers[self:getCurrentState()]
end

function PlayStateMachine:_changeState(msg)
	if msg ~= nil then
		print(self:getCurrentState(), msg)
		self.sm:onMessage(msg)
		print(self:getCurrentState())
	end
end

function PlayStateMachine:_process(cmd, ...)
	local state = self:getCurrentState()
	
	if state ~= 'Finish' then
		local controller = self.stateControllers[state]
		local msg = controller[cmd](controller, ...)
		self:_changeState(msg)
	end
end

function PlayStateMachine:_onCommand(cmd, x, y)
	if self.runner then return end
	
	self.runner = coroutine.wrap(function ()
		self:_process(cmd, x, y)
		self.runner = nil
	end)
	
	self.runner()
end

function PlayStateMachine:onHover(x, y)
	if not self.runner then 
		return self:_process('onHover', x, y)
	end
end

function PlayStateMachine:onTouch(x, y)
	return self:_onCommand('onTouch', x, y)
end

function PlayStateMachine:onBack()
	return self:_onCommand('onBack')
end

function PlayStateMachine:onUICommand(cmd)
	return self:_onCommand('onUICommand', cmd)
end

function PlayStateMachine:nextHero()
	self:_changeState('continue')
end

if select('#', ...) == 0 then 
	local instance = PlayStateMachine.new()
	assert(instance:getCurrentState() == instance.sm:getCurrentState())
end

return PlayStateMachine

