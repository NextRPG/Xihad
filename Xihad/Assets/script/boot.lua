require 'Assets.script.AllPackages'	-- change package.path
require 'math3d'					-- load math3d
require 'IrrlichtRender'			-- load Irrlicht render module
require 'CreateMesh'				-- create cube mesh
require 'cegui'						-- load cegui module
require 'XihadWarrior'				-- init warrior fields

------------------------------------------------------------------------------
local Painter = require 'Chessboard.Painter'
local UIFacade= require 'GUI.UIFacade'
local GUISystem   = require 'GUI.GUISystem'
local functional  = require 'std.functional'
local sCoroutine  = require 'std.sCoroutine'
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


------------------------------------------------------------------------------
-- create g_scheduler
g_scheduler = TaskScheduler.new()
g_scene:appendUpdateHandler {
	onUpdate = functional.bindself(g_scheduler, 'onUpdate')
}
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- init gui module
g_cursor:setVisible(false)
local guiSystemController = GUISystem.init()
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- ADD LIGHT
local sun = g_scene:createObject(c'sun')
local lightControl = sun:appendComponent(c'Light')
lightControl:castShadow(false)
lightControl:setType 'direction'
sun:concatTranslate(math3d.vector(0, 30, 0))
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- Load level
local battle = dofile('Assets/level/level_01.battle')
local heros  = dofile('User/sav/Save1.hero')
local teamFactory = { 
	Enemy= WarriorFactory.new(), 
	Hero = WarriorFactory.new(),
}
local loader = LevelFactory.new(teamFactory, heros)
g_chessboard = loader:create(battle)
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- init player state machine
local uiFacade = UIFacade.new()
local cameraFacade = CameraFacade.new(CameraFactory.createDefault('camera'))
local painter = Painter.new()
local cmdExecutor = CommandExecutor.new(cameraFacade, ExpCalculator.new())
local stateMachine= PlayerStateMachine.new(uiFacade, cameraFacade, painter, cmdExecutor)
uiFacade:setCommandReceiver(stateMachine)

-- Auto hide cursor
stateMachine:setBlockListener({
		onBlocked = function(self, sm, blocked)
			local g_GUICursor = require 'GUI.GUICursor'
			g_GUICursor:setVisible(not blocked)
		end
	})
------------------------------------------------------------------------------


------------------------------------------------------------------------------
--- init controllers
local controller = ControllerAdapter.new(PCInputTransformer.new(stateMachine))
g_scene:pushController(controller)
controller:drop()
g_scene:pushController(guiSystemController)
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- set battle victory condition
local battleManager = BattleManager.new()
local heroTeam = XihadBattleTeam.new('Hero', PlayerController.new(stateMachine))
local enemyTeam= XihadBattleTeam.new('Enemy', AIController.new(cmdExecutor))
battleManager:addTeam(heroTeam)
battleManager:addTeam(enemyTeam)

local ConditionFactory = require 'Condition.ConditionFactory'
battleManager:addVictoryCondition(
	ConditionFactory.beatWarriorsWithTag('Enemy'), 'Hero')

battleManager:addVictoryCondition(
	ConditionFactory.beatWarriorsWithTag('Hero'), 'Enemy')
------------------------------------------------------------------------------

local XihadConversation = require 'Conversation.XihadConversation'
local BaseCondition = require 'Condition.BaseCondition'
local cond = BaseCondition.new()
function cond:_updateCondition()
	-- return true
	return battleManager:getCurrentRound() == 2
end

battleManager:addCommandCondition(cond, 
	function()
		local conv = XihadConversation.new()
		local aSpeaker = conv:addWarrior('A')
		local bSpeaker = conv:addWarrior('B')
		
		conv:start()
		
		--- close -(O)-> deactive -(S)-> active -(D)-> deactive -(C)-> close
		aSpeaker:speak('等等')
		bSpeaker:speak('caonima')
		aSpeaker:speak({'hahah', 'hhhh'})
		
		conv:stop()
	end)

------------------------------------------------------------------------------
-- Test script
local InputSimulator = require 'Controller.InputSimulator'
local simulator = InputSimulator.new(stateMachine)
-- simulator:selectWarrior('A')
-- -- simulator:selectTile(aTile)
-- simulator:selectTileAt(8, 1)
-- simulator:selectCommand('交换')
-- simulator:selectCommand('技能', 'Fire')
-- simulator:selectTileAt(8, 2)
-- -- simulator:selectTile(aTile)
-- -- simulator:selectCommand('道具', '长矛')
-- -- simulator:selectCommand('道具', '长矛')
-- simulator:selectCommand('待机')
-- simulator:selectWarrior('D')
-- simulator:selectTileAt(8, 2)
-- simulator:selectCommand('待机')

-- g_world:setTimeScale(0.1)

g_camera = cameraFacade
battleManager:startBattle()
