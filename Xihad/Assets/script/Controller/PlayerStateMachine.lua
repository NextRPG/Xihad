local Class = require 'std.Class'
local Array = require 'std.Array'
local sCoroutine = require 'std.sCoroutine'
local CommandList = require 'Command.CommandList'
local StateMachine = require 'std.StateMachine'
local CursorFastener  = require 'Controller.CursorFastener'
local ChooseHeroState = require 'Controller.ChooseHeroState'
local ChooseTileState = require 'Controller.ChooseTileState'
local ChooseTargetState = require 'Controller.ChooseTargetState'
local ChooseCommandState = require 'Controller.ChooseCommandState'
local CursorEventDispatcher = require 'Controller.CursorEventDispatcher'

local PlayStateMachine = {
	sm = nil,
	cmdList= nil,
	runner = nil,
	stateControllers = nil,
	inputBlockListener = nil,
	
	postCommands = nil,
	
	touchDispatcher = nil,
	hoverDispatcher = nil,
}
PlayStateMachine.__index = PlayStateMachine

function PlayStateMachine.new(...)
	local obj = setmetatable({
			sm = StateMachine.new('ChooseHero'),
			cmdList = CommandList.new(),
			stateControllers = {},
			postCommands = {},
		}, PlayStateMachine)
	
	obj:_initTransitions()	
	
	local cursorFastener = CursorFastener.new()
	
	local commandList = obj.cmdList	
	obj.stateControllers['ChooseHero'] 	 = ChooseHeroState.new(commandList, cursorFastener, ...)
	obj.stateControllers['ChooseTile'] 	 = ChooseTileState.new(commandList, cursorFastener, ...)
	obj.stateControllers['ChooseCommand']= ChooseCommandState.new(commandList, cursorFastener, ...)
	obj.stateControllers['ChooseTarget'] = ChooseTargetState.new(commandList, cursorFastener, ...)
	
	for stateName, state in pairs(obj.stateControllers) do
		obj.sm:addStateListener(stateName, state)
	end
	
	local touchListener = {}
	Class.delegateClosure(touchListener, 'needCollisionDetection', obj, 'needCDWhenTouch')
	Class.delegateClosure(touchListener, 'onWarrior', obj, 'onWarriorSelected')
	Class.delegateClosure(touchListener, 'onTile' ,   obj, 'onTileSelected')
	obj.touchDispatcher = CursorEventDispatcher.new(touchListener, cursorFastener)
	
	local hoverListener = {}
	Class.delegateClosure(hoverListener, 'needCollisionDetection', obj, 'needCDWhenHover')
	Class.delegateClosure(hoverListener, 'onWarrior', obj, 'onWarriorHovered')
	Class.delegateClosure(hoverListener, 'onTile' ,   obj, 'onTileHovered')
	obj.hoverDispatcher = CursorEventDispatcher.new(hoverListener, cursorFastener)
	
	return obj
end

function PlayStateMachine:_initTransitions()
	local sm = self.sm
	
	sm:setTransition('ChooseHero', 'next', 'ChooseTile')
	
	sm:setTransition('ChooseTile', 'next', 'ChooseCommand')
	sm:setTransition('ChooseTile', 'back', 'ChooseHero')
	
	sm:setTransition('ChooseCommand', 'next', 'ChooseTarget')
	sm:setTransition('ChooseCommand', 'done', 'Finish')
	sm:setTransition('ChooseCommand', 'back', 'ChooseTile')
	
	sm:setTransition('ChooseTarget', 'next', 'Finish')
	sm:setTransition('ChooseTarget', 'back', 'ChooseCommand')
	
	---
	-- @see nextHero()
	sm:setTransition('Finish', 'continue', 'ChooseHero')
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

function PlayStateMachine:setBlockListener(blockListener)
	self.inputBlockListener = blockListener
end

function PlayStateMachine:_setRunner(runner)
	self.runner = runner
	
	if self.inputBlockListener then
		local blocked = (runner ~= nil)
		self.inputBlockListener:onBlocked(self, blocked)
	end
	
	if not self.runner and not Array.empty(self.postCommands) then
		local cmd = Array.removeElementAt(self.postCommands, 1)
		g_scheduler:schedule(function()
				self:onCommand(unpack(cmd))
			end)
	end
end

function PlayStateMachine:postCommand(...)
	if self.runner then		
		table.insert(self.postCommands, {...})
	else
		self:onCommand(...)
	end
end

function PlayStateMachine:onCommand(cmd, ...)
	if self.runner then return end
	
	self:_setRunner(coroutine.create(function (...)
		self:_process(cmd, ...)
		self:_setRunner(nil)
	end))
	
	sCoroutine.resume(self.runner, ...)
end

function PlayStateMachine:getCommandList()
	return self.cmdList
end

function PlayStateMachine:onHover(x, y)
	if self.runner then return end
	
	if self:_getCurrentStateController():needHover() then
		self.hoverDispatcher:dispatch(x, y)
	end
end

function PlayStateMachine:needCDWhenHover()
	return self:_getCurrentStateController():needCDWhenHover()
end

function PlayStateMachine:onWarriorHovered(obj)
	return self:_process('onWarriorHovered', obj)
end

function PlayStateMachine:onTileHovered(tile)
	return self:_process('onTileHovered', tile)
end

function PlayStateMachine:onTouch(x, y, times)
	if self.runner then return end
		
	if self:_getCurrentStateController():needTouch() then
		self.touchDispatcher:dispatch(x, y, times)
	end
end

function PlayStateMachine:needCDWhenTouch()
	return self:_getCurrentStateController():needCDWhenTouch()
end

function PlayStateMachine:onWarriorSelected(obj, times)
	return self:onCommand('onWarriorSelected', obj, times)
end

function PlayStateMachine:onTileSelected(tile, times)
	return self:onCommand('onTileSelected', tile, times)
end

function PlayStateMachine:onBack()
	return self:onCommand('onBack')
end

function PlayStateMachine:onUICommand(...)
	return self:onCommand('onUICommand', ...)
end

function PlayStateMachine:nextHero()
	self:_changeState('continue')
end

if select('#', ...) == 0 then 
	local instance = PlayStateMachine.new()
	assert(instance:getCurrentState() == instance.sm:getCurrentState())
end

return PlayStateMachine

