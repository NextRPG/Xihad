--- 
-- 负责游戏中Manager的初始化，初始化顺序很重要
-- @module boot
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

require 'Assets.script.AllPackages'
global = g_scene:createObject(c("global"))

require "Consts"
require "LuaUtils"
require "math3d"

g_scene:requireSystem(c"Render")	-- load Irrlicht render component system
local colorMesh = g_geometry:createCube(Consts.TILE_WIDTH - 1, 0.5, Consts.TILE_HEIGHT - 1)
g_meshManager:takeMesh("@colorCube", colorMesh)
local cubeMesh = g_geometry:createCube(Consts.TILE_WIDTH, 5, Consts.TILE_HEIGHT)
g_meshManager:takeMesh("@chessboardCube", cubeMesh)
-- g_meshManager:takeMesh(id, mesh) 
-- 等价于
-- g_meshManager:addMesh(id, mesh); 	mesh:drop()


local AIManager      = require "AIManager"
local ColoringManager= require "ColoringManager"
local Chessboard 	 = require "Chessboard"
local HeroManager    = require "HeroManager"
local LightManager   = require "LightManager"
local CameraManager  = require "CameraManager"
local BattleManager  = require "BattleManager"
local VictoryChecker = require "VictoryChecker"

-- load resources
require "SkillDatabase"
require "StrategyDatabase"

-- load save files
local battle = dofile("Assets/level/level_01.battle")
local heros = dofile("User/sav/Save1.hero")


-- init battle related manager
ColoringManager:init(battle.chessboard)
g_chessboard = Chessboard.new(Consts.TILE_WIDTH, Consts.TILE_HEIGHT)
-- g_chessboard:traverseTiles()

HeroManager:init(battle.heros, heros)
AIManager:init(battle.AIs)
BattleManager:init(HeroManager, AIManager)
VictoryChecker:init()

-- init Camera related manager
CameraManager:init()
LightManager:init()

-- init Controller
local gameController = require("InputController")
g_scene:pushController(gameController) 
gameController:drop()

g_world:setTimeScale(2)
