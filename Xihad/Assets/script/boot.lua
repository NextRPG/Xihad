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
local TileInfoView= require 'GUI.TileInfoView'
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
-- local SpanColor = require 'Action.SpanColor'
-- local ActionAdapter= require 'Action.ActionAdapter'
-- local LightAction = require 'HighAction.LightAction'

-- local spanColor = SpanColor.new(Color.new(0), Color.new(0xffffffff))
-- local action = LightAction.diffuse(lightControl, spanColor, 1)
-- ActionAdapter.fit(sun, action)

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

local cmdExecutor = CommandExecutor.new(cameraFacade)

local stateMachine= PlayerStateMachine.new(ui, cameraFacade, painter, cmdExecutor)
local controller = ControllerAdapter.new(PCInputTransformer.new(stateMachine))
g_scene:pushController(controller)
controller:drop()

stateMachine:setBlockListener({
		onBlocked = function(self, sm, blocked)
			local guiCursor = require 'GUI.GUICursor'
			guiCursor:setVisible(not blocked)
		end
	})

local function showRoundInfo( )
	-- local AsConditionFactory = require 'Async.AsConditionFactory'
	-- local spanColor = SpanColor.new(nil, Color.new(0xff2b2b2b))
	-- local action = LightAction.diffuse(lightControl, spanColor, 1)
	-- ActionAdapter.fit(sun, action)
	-- AsConditionFactory.waitAction(action)
	
	-- spanColor:flip()
	-- local action = LightAction.diffuse(lightControl, spanColor, 1)
	-- ActionAdapter.fit(sun, action)
	-- AsConditionFactory.waitAction(action)
end

local function startEnemy()
	showRoundInfo()
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
	
	showRoundInfo()
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

-- Test Parcel
local aObject = g_scene:findObject(c'A')
local aTile
if aObject then
	aTile = aObject:findComponent(c'Barrier'):getTile()
end

local InputSimulator = require 'Controller.InputSimulator'
local simulator = InputSimulator.new(stateMachine)
simulator:selectWarrior('B')
-- simulator:selectTileAt(8, 6)
-- simulator:selectCommand('技能', 'Fire')
-- simulator:selectTile(aTile)
-- simulator:selectCommand('道具', '长矛')
-- simulator:selectCommand('道具', '长矛')
-- simulator:selectCommand('待机')


-- g_world:setTimeScale(3)
