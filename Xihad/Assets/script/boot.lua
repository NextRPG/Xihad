require 'Assets.script.AllPackages'	-- change package.path

require 'math3d'					-- load math3d
require 'IrrlichtRender'			-- load Irrlicht render module
require 'CreateMesh'				-- create cube mesh
require 'cegui'						-- load cegui module

local Warrior = require 'XihadWarrior'
local Painter = require 'Chessboard.Painter'
local GUISystem   = require 'GUI.GUISystem'
local functional  = require 'std.functional'
local sCoroutine  = require 'std.sCoroutine'
local CommandView = require 'GUI.CommandView'
local TaskScheduler = require 'Scheduler.TaskScheduler'
local LevelFactory 	= require 'Level.XihadLevelFactory'
local WarriorFactory= require 'Warrior.WarriorFactory'
local CameraFactory = require 'Camera.SimpleCameraFactory'
local CameraFacade  = require 'Camera.CameraFacade'
local CommandExecutor = require 'Command.CommandExecutor'
local PCInputTransformer = require 'Controller.PCInputTransformer'
local PlayerStateMachine = require 'Controller.PlayerStateMachine'
local ControllerAdapter  = require 'Controller.ControllerAdapter'

--- Code for debug coroutines
-- local resume = coroutine.resume
-- coroutine.resume = function(co, ...)
-- 	print('resuming from ', coroutine.running(), ' to ', co)
-- 	return resume(co, ...)
-- end

-- ADD LIGHT
local sun = g_scene:createObject(c'sun')
local lightControl = sun:appendComponent(c'Light')
lightControl:castShadow(false)
lightControl:setType 'direction'
sun:concatTranslate(math3d.vector(0, 30, 0))

-- create g_scheduler
g_scheduler = TaskScheduler.new()
g_scene:appendUpdateHandler {
	onUpdate = functional.bindself(g_scheduler, 'onUpdate')
}

-- Load level
local battle = dofile('Assets/level/level_01.battle')
local heros  = dofile('User/sav/Save1.hero')
local warriorFactory = WarriorFactory.new()
local loader = LevelFactory.new({ Enemy = warriorFactory, Hero = warriorFactory }, heros)
g_chessboard = loader:create(battle)

-- INPUT
local ui = {
	showWarriorInfo = function (self, warrior)
		print('ui info: ', warrior:getHostObject():getID())
	end,
	
	showTileInfo = function (self, tile)
		print('ui info: ', tile:getTerrain().type)
	end,
	
	warning = function (self, msg)
		print(msg)
	end,
}

local cameraFacade = CameraFacade.new(CameraFactory.createDefault('camera'))

local painter = Painter.new()

local cmdExecutor = CommandExecutor.new(cameraFacade)

local stateMachine= PlayerStateMachine.new(ui, cameraFacade, painter, cmdExecutor)
local controller = ControllerAdapter.new(PCInputTransformer.new(stateMachine))
g_scene:pushController(controller)
controller:drop()

local function startEnemy()
	print('player round over')
	for object in g_scene:objectsWithTag('Enemy') do
		local warrior = object:findComponent(c'Warrior')
		warrior:activate()
	end
	
	for object in g_scene:objectsWithTag('Enemy') do
		local tactic = object:findComponent(c'Tactic')
		local cmdList = tactic:giveOrder()
		print(tostring(cmdList))
		cmdExecutor:execute(cmdList)
	end
	
	print('player round begin')
	for object in g_scene:objectsWithTag('Hero') do
		local warrior = object:findComponent(c'Warrior')
		warrior:activate()
	end
	
	local hasActive = false
	for object in g_scene:objectsWithTag('Hero') do
		local warrior = object:findComponent(c'Warrior')
		if warrior:isActive() then
			hasActive = true
			break
		end
	end
	
	if hasActive then
		stateMachine:nextHero()
	else
		startEnemy()
	end
end

local finishListener = {}
function finishListener:onStateEnter(state, prev)
	assert(state == 'Finish', state)
	for object in g_scene:objectsWithTag('Hero') do
		local warrior = object:findComponent(c'Warrior')
		if warrior:isActive() then
			stateMachine:nextHero()
			return
		end
	end
	
	startEnemy()
end

function finishListener:onStateExit(state, next) end

stateMachine:addStateListener('Finish', finishListener)

-- init gui module
g_cursor:setVisible(false)
GUISystem.init()

-- Command View
CommandView.hook(stateMachine)

local enemyRound = false
if not enemyRound then
	for heroObj in g_scene:objectsWithTag('Hero') do
		heroObj:findComponent(c'Warrior'):activate()
	end
else
	sCoroutine.start(startEnemy)
end

-- g_world:setTimeScale(0.3)
