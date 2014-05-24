local XihadTileSet = {}

function XihadTileSet.create(locations, filter, set)
	set = set or {}
	for loc in locations() do
		local tile = g_chessboard:getTile(loc)
		if tile and filter(tile) then
			set[tile] = true
		end
	end
	
	return set
end

return XihadTileSet