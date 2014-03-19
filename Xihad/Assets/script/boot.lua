--- 
-- 负责游戏中Manager的初始化，初始化顺序很重要
-- @module boot
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

package.path = package.path 
.. ";Assets/Script/?.lua"
.. ";Assets/Script/Action/?.lua" 
.. ";Assets/Script/Effect/?.lua"
.. ";Assets/Script/Utils/?.lua"
.. ";Assets/Script/Save/?"
.. ";Assets/Script/Database/?.lua"
.. ";Assets/Script/AI/?.lua"
.. ";Assets/Script/Component/?.lua"
.. ";Assets/Script/IO/?.lua"
.. ";Assets/Script/Manager/?.lua"
.. ";Assets/Script/Lib/?.lua"

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


local Chessboard = require "Chessboard"
local HeroManager = require "HeroManager"
local AIManager = require "AIManager"
local CameraManager = require "CameraManager"
local BattleManager = require "BattleManager"
local LightManager = require "LightManager"
local VictoryChecker = require "VictoryChecker"

-- load resources
require "SkillDatabase"
require "StrategyDatabase"

-- load save files
local CUR_DIR = debug.getinfo(1).source:gsub("^@", ""):gsub("[^\\\/]*$", ""):gsub("[^\\\/]$", "%1\\")
local battle = dofile(CUR_DIR .. "\\Save\\level_01.battle")
local heros = dofile(CUR_DIR .. "\\Save\\Save1.hero")


-- init battle related manager
Chessboard:init(battle.chessboard)
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
