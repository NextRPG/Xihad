local TerrainEffect = { }
TerrainEffect.__index = TerrainEffect

function TerrainEffect.new()
	local obj = setmetatable({ }, TerrainEffect)
	return obj
end

return TerrainEffect