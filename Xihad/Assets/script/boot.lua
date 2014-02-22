--- 
-- 负责游戏中Manager的初始化，初始化顺序很重要
-- @module boot
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

package.path = package.path 
.. ";Assets/Script/Action/?.lua" 
.. ";Assets/Script/Effect/?.lua"
.. ";Assets/Script/Utils/?.lua"
.. ";Assets/Script/Save/?"
.. ";Assets/Script/Database/?.lua"
.. ";Assets/Script/AI/?.lua"
.. ";Assets/Script/Component/?.lua"
.. ";Assets/Script/IO/?.lua"
.. ";Assets/Script/Manager/?.lua"


require "Consts"
require "LuaUtils"
require "math3d"

local Chessboard = require "Chessboard"
local HeroManager = require "HeroManager"
local AIManager = require "AIManager"
local CameraManager = require "CameraManager"
local BattleManager = require "BattleManager"
local LightManager = require "LightManager"

-- load resources
require "SkillDatabase"

-- load save files
local CUR_DIR = debug.getinfo(1).source:gsub("^@", ""):gsub("[^\\\/]*$", ""):gsub("[^\\\/]$", "%1\\")
local battle = dofile(CUR_DIR .. "\\Save\\maptest.battle")
local battle = dofile(CUR_DIR .. "\\Save\\level_01.battle")

-- init battle related manager
Chessboard:init(battle.chessboard)
HeroManager:init(battle.heros)
AIManager:init(battle.AIs)
BattleManager:init(HeroManager, AIManager)

-- init Camera related manager
CameraManager:init()
LightManager:init()


-- init Controller
scene:pushController(require("InputController"))

cursor:setVisible(true)