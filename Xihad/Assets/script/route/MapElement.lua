local MapElement = {
	tile = nil
}
MapElement.__index = MapElement

function MapElement.new()
	local o = {}
	setmetatable(o, MapElement)
	return o
end

function MapElement:canPass( character )
	return true
end

function MapElement:canStay( character )
	return true
end

function MapElement:permitCasting( character, skill )
	return false
end

function MapElement:getActionPointCost( character )
	return 0
end

function MapElement:getTile( )
	return self.tile
end

function MapElement:setTile( tile ) 
	if self.tile == tile then return end
	
	if self.tile then
		self.tile = nil
		self.tile:onElementRemoved(self)
	end
	
	self.tile = tile
	
	if self.tile then
		self.tile:onElementAdded(self)
	end
end

return MapElement
