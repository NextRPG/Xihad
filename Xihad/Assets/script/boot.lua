require 'Assets.script.AllPackages'	-- change package.path
require 'math3d'					-- load math3d
require 'IrrlichtRender'			-- load Irrlicht render module
require 'CreateMesh'				-- create cube mesh
require 'cegui'						-- load cegui module
require 'XihadWarrior'				-- init warrior fields

------------------------------------------------------------------------------
local Painter = require 'Chessboard.Painter'
local GUISystem   = require 'GUI.GUISystem'
local functional  = require 'std.functional'
local sCoroutine  = require 'std.sCoroutine'
local TileInfoView= require 'GUI.TileInfoView'
local CommandView = require 'GUI.CommandView'
local TaskScheduler = require 'Scheduler.TaskScheduler'
local LevelFactory 	= require 'Level.XihadLevelFactory'
local WarriorFactory= require 'Warrior.WarriorFactory'
local ExpCalculator = require 'Warrior.ExpCalculator'
local CameraFactory = require 'Camera.SimpleCameraFactory'
local CameraFacade  = require 'Camera.CameraFacade'
local CommandExecutor = require 'Command.CommandExecutor'
local PCInputTransformer = require 'Controller.PCInputTransformer'
local PlayerStateMachine = require 'Controller.PlayerStateMachine'
local ControllerAdapter  = require 'Controller.ControllerAdapter'
------------------------------------------------------------------------------
local BattleManager= require 'GameFlow.BattleManager'
local AIController = require 'GameFlow.AIController'
local PlayerController= require 'GameFlow.PlayerController'
local XihadBattleTeam = require 'GameFlow.XihadBattleTeam'
------------------------------------------------------------------------------
------------------------------------------------------------------------------

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
local teamFactory = { 
	Enemy= WarriorFactory.new(), 
	Hero = WarriorFactory.new(),
}
local loader = LevelFactory.new(teamFactory, heros)
g_chessboard = loader:create(battle)

-- INPUT
local ui = {
	showWarriorInfo = function (self, warrior)
		print('ui info: ', warrior:getHostObject():getID())
	end,
	
	showTileInfo = function (self, tile)
		if tile then
			print(tostring(tile:getLocation()))
			TileInfoView.show(tile)
		else
			TileInfoView.hide()
		end
	end,
	
	warning = function (self, msg)
		print(msg)
	end,
}

local cameraFacade = CameraFacade.new(CameraFactory.createDefault('camera'))
g_camera = cameraFacade

local painter = Painter.new()

local cmdExecutor = CommandExecutor.new(cameraFacade, ExpCalculator.new())

local stateMachine= PlayerStateMachine.new(ui, cameraFacade, painter, cmdExecutor)
local controller = ControllerAdapter.new(PCInputTransformer.new(stateMachine))
g_scene:pushController(controller)
controller:drop()

-- init gui module
g_cursor:setVisible(false)
GUISystem.init()

-- Auto hide cursor
stateMachine:setBlockListener({
		onBlocked = function(self, sm, blocked)
			local g_GUICursor = require 'GUI.GUICursor'
			g_GUICursor:setVisible(not blocked)
		end
	})

local battleManager = BattleManager.new()
local heroTeam = XihadBattleTeam.new('Hero', PlayerController.new(stateMachine))
local enemyTeam = XihadBattleTeam.new('Enemy', AIController.new(cmdExecutor))
battleManager:addTeam(heroTeam)
battleManager:addTeam(enemyTeam)
battleManager:startBattle()

local VictoryConditionFactory = require 'Victory.VictoryConditionFactory'
battleManager:addVictoryCondition(
	VictoryConditionFactory.beatWarriorsWithTag('Enemy'), 'Hero')

battleManager:addVictoryCondition(
	VictoryConditionFactory.beatWarriorsWithTag('Hero'), 'Enemy')

-- Command View
CommandView.hook(stateMachine)

-- Test Parcel
local InputSimulator = require 'Controller.InputSimulator'
local simulator = InputSimulator.new(stateMachine)
simulator:selectWarrior('A')
-- -- simulator:selectTile(aTile)
-- simulator:selectTileAt(3, 6)
-- simulator:selectCommand('技能', 'Fire')
-- simulator:selectTileAt(3, 7)
-- -- simulator:selectTile(aTile)
-- -- simulator:selectCommand('道具', '长矛')
-- -- simulator:selectCommand('道具', '长矛')
-- simulator:selectCommand('待机')
-- simulator:selectWarrior('D')
-- simulator:selectTileAt(8, 2)
-- simulator:selectCommand('待机')

-- g_world:setTimeScale(3)
