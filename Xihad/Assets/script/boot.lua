g_scene:requireSystem(c"Render")	-- load Irrlicht render component system
local levelPath = 'Assets/level/level_01.battle'
local userSavePath = 'User/sav/Save1.hero'

require 'Assets.script.AllPackages'	-- change package.path
require 'math3d'					-- load math3d
require "CreateMesh"				-- create cube mesh

local Ability = require 'Warrior.Ability'
local LevelFactory = require 'Level.XihadLevelFactory'
local WarriorFactory = require 'Warrior.WarriorFactory'

-- register game specific properties
Ability.registerProperty('MHP')
Ability.registerProperty('ATK')
Ability.registerProperty('DFS')
Ability.registerProperty('MTK')
Ability.registerProperty('MDF')
Ability.registerProperty('MAP')

local battle = dofile(levelPath)
local heros  = dofile(userSavePath)
local warriorFactory = WarriorFactory.new()
local loader = LevelFactory.new({ Enemy = warriorFactory, Hero = warriorFactory }, heros)
g_chessboard = loader:create(battle)

local SRPGCamera = require 'Camera.SRPGCamera'
g_camera = SRPGCamera.new('camera')

-- TEST ROUTE
local Location = require 'Location'
local aHero = g_scene:findObject(c'A')
local startLoc = aHero:findComponent(c'Barrier'):getTile():getLocation()
local locations = g_chessboard:route(aHero:findComponent(c'Ability'), startLoc, Location.new(10, 2))
for _, loc in ipairs(locations) do
	print(loc:xy())
end

-- TEST ITERATOR


-- ADD LIGHT
local sun = g_scene:createObject(c'sun')
lightComp = sun:appendComponent(c'Light')
lightComp:castShadow(true)
lightComp:setType "point"
sun:concatRotate(math3d.vector(90, 0, 0))
sun:concatTranslate(math3d.vector(20, 30, -5))
