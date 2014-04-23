local Barrier = require 'Barrier'
local TerrainDB  = require 'TerrainDatabase'

local Terrain = {
	type = nil,
}
Terrain.__index = Terrain
Terrain.__base = 'Barrier'
setmetatable(Terrain, Barrier)

function Terrain.new(type, object)
	local o = Barrier.new()
	setmetatable(o, Terrain)
	assert(type and TerrainDB.passable[type])
	o.type = type
	
	-- TODO DELETE
	local Color = require 'Color'
	local mat = object:findComponent(c'Render'):getMaterial(0)
	mat:setColorMaterial('none')
	local colorTable = { Color.green, Color.blue, Color.red, }
	mat:setDiffuseColor(Color.new(colorTable[type]))
	
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
		Barrier.setTile(self, tile)
		self.object:resetTranslate(tile:getCenterVector())
	else
		error("Attempt to change the MapTile of a Terrain")
	end
end

function Terrain.highlight(terrianList, color)
	for _, terrian in ipairs(locationList) do
		local render = terrian.object:findComponent(c'Render')
		for i = 0, render:getMaterialCount()-1 do
			local mat = render:getMaterial(i)
			mat:setDiffuseColor(color)
		end
	end
end

return Terrain
