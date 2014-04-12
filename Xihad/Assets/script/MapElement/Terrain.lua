local MapElement = require 'MapElement'
local TerrainDB  = require 'TerrainDatabase'

local Terrain = {
	type = nil,
}
Terrain.__index = Terrain
setmetatable(Terrain, MapElement)

function Terrain.new(type)
	local o = MapElement.new()
	setmetatable(o, Terrain)
	assert(type and TerrainDB.passable[type])
	self.type = type
	return o
end

local function selectResult(field, type, character)
	local dedicated = TerrainDB[field][type][character:getCareer()] 
	
	if dedicated ~= nil then
		return dedicated
	else
		return TerrainDB[field][type]['*']
	end
end

function Terrain:canPass( character )
	return selectResult('passable', self.type, character)
end

function Terrain:canStay( character )
	return selectResult('stayable', self.type, character)
end

function Terrain:getActionPointCost( character )
	return selectResult('apcost', self.type, character)
end

function Terrain:setTile( tile ) 
	if not self.tile then
		MapElement.setTile(self, tile)
	else
		error("Attempt to change the MapTile of a Terrain")
	end
end

return Terrain
