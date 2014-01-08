local TerrainDatabase = {
	
}

function loadTerrain( terrain )
	TerrainDatabase[terrain.id] = terrain
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
	canStay = true,
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