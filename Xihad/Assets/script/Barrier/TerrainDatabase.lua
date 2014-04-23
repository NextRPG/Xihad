local TerrainDatabase = {
	passable = {},
	stayable = {},
	apcost = {},
}

local function getFiled(forID, field)
	local ret = TerrainDatabase[field][forID]
	if not ret then
		ret = {}
		TerrainDatabase[field][forID] = ret
	end
	
	return ret
end

local function loadTerrain(data)
	local passable =  getFiled(data.id, 'passable')
	passable['*'] = data.canPass
	
	local stayable =  getFiled(data.id, 'stayable')
	stayable['*'] = data.canStay
	
	local apcost =  getFiled(data.id, 'apcost')
	apcost['*'] = data.APcost
end

loadTerrain{
	id = 1,
	name = "平原",
	canPass = true,
	canStay = true,
	APcost = 1
}

loadTerrain{
	id = 2,
	name = "河流",
	canPass = false,
	canStay = false,
	APcost = 1
}

loadTerrain{
	id = 3,
	name = "树林",
	canPass = true,
	canStay = true,
	APcost = 2
}

return TerrainDatabase