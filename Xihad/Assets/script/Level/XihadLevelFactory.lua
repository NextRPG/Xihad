local base = require 'Level.BaseLevelFactory'
local Terrain = require 'Barrier.Terrain'
local Chessboard = require 'route.Chessboard'
local XihadMapTile = require 'Chessboard.XihadMapTile'
local WarriorFactory = require 'Warrior.WarriorFactory'

local XLoader = {
	teamFactories = nil,
	loadedHeroData= nil,
	idx = 0,
}
XLoader.__index = XLoader
setmetatable(XLoader, base)

function XLoader.new(teamFactories, loadedHeroData)
	local obj = base.new()
	setmetatable(obj, XLoader)
	
	assert(type(teamFactories) == 'table')
	obj.teamFactories = teamFactories
	obj.loadedHeroData = loadedHeroData
	return obj
end

function XLoader:createChessboard(width, height)
	return Chessboard.new(width, height, XihadMapTile.new)
end

function XLoader:createTerrian(terrainType)
	-- no parent and add Highlight to tag
	local highlightObject = g_scene:createUniqueObject(c'Highlight')
	local meshComponent = highlightObject:appendComponent(c'Mesh')
	meshComponent:setMesh(g_meshManager:getMesh('@colorCube'))
	
	return highlightObject:appendComponent(c'Terrain', terrainType)
end

function XLoader:_checkTeam(team)
	if not self.teamFactories[team] then
		error(string.format('no factory for team: %s', team))
	end
end

function XLoader:createPrefab(team, data, name)
	self:_checkTeam(team)
	self.idx = self.idx + 1
	
	name = name or data.career..self.idx
	return self.teamFactories[team]:create(team, name, data)
end

function XLoader:createNamed(team, name)
	if not self.loadedHeroData[name] then
		print(string.format('no loaded data for: %s', name))
	else
		return self:createPrefab(team, self.loadedHeroData[name], name)
	end
end

return XLoader