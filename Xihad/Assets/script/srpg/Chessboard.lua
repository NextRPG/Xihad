local Location = require 'Location'

local Chessboard = {
	width = 1,
	height= 1,
	tiles = nil,
}

function function_name()
	
end

function BoardDelegate:computeCost(x1, y1, x2, y2)
	local tile = self:getTile(Location.new(x2, y2))
	local cost = tile:getActionPointCost(self.routingCharacter)
	
	assert(cost >= 1)
	return cost
end