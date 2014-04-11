local algo = require 'std.Algorithm'

local MapTile = {
	location = nil,	-- location of this tile
	elements = nil,
}
MapTile.__index = MapTile

function MapTile.new(location)
	local tile = {}
	setmetatable(tile, MapTile)
	tile.location = location:copy()
	tile.elements = {}
	return tile
end

function MapTile:getLocation()
	return self.location
end

function MapTile:onElementAdded(e)
	if e:getTile() == self then
		self.elements[e] = true
	end
end

function MapTile:onElementRemoved(e)
	if e:getTile() ~= self then
		self.elements[e] = nil
	end
end

function MapTile:permitCasting(character, skill)
	return algo.any_of(self.elements, function(e, v)
		return e:permitCasting(character, skill)
	end)
end

function MapTile:canPass(character)
	return algo.all_of(self.elements, function(e, v)
		return e:canPass(character)
	end)
end

function MapTile:canStay(character)
	return algo.all_of(self.elements, function(e, v)
		return e:canStay(character)
	end)
end

function MapTile:getActionPointCost(character)
	local accum = 0
	for e,_ in pairs(self.elements) do
		accum = accum + self.elements:getActionPointCost(character)
	end
	
	return algo.max(1, accum)
end

return MapTile
