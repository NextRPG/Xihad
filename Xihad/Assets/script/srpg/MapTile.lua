local MapTile = {
	location = nil,	-- location of this tile
	elements = nil,
}

function MapTile:adopt(mapElement)
	local tile = mapElement:getTile()
	if tile then
		table.remove(tile.elements, mapElement)
	end
	
	
end

