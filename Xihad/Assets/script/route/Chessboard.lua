require 'ArtIn'
local Array = require 'std.Array'
local MapTile  = require 'MapTile'
local Location = require 'Location'

local Chessboard = {
	width = 20,
	height= 20,
	tiles = nil,
	graph = nil,
	delegate = nil,
}
Chessboard.__index = Chessboard

local function toIndex(location, width)
	return (location.y-1)*width + location.x
end

function Chessboard.new(width, height, mapTileFactory)
	local b = {}
	b.width = width
	b.height= height
	b.tiles = {}
	
	b.delegate = {
		computeCost = function(self, x1, y1, x2, y2)
			local tile = b:getTile(Location.new(x2, y2))
			if not tile:canPass(self.routingCharacter) then
				return 10000	-- TODO
			end
			
			local cost = tile:getActionPointCost(self.routingCharacter)
			
			assert(cost >= 1)
			return cost
		end
	}
	
	b.graph = Astar.newChessboard(b.width, b.height, b.delegate)
	setmetatable(b, Chessboard)
	
	mapTileFactory = mapTileFactory or MapTile.new
	b:traverseLocations(function (loc)
		b.tiles[toIndex(loc, b.width)] = mapTileFactory(loc)
	end)
	return b
end

function Chessboard:getTile(location)
	local index = toIndex(location, self.width)
	
	if index < 0 or index > #self.tiles then
		return nil
	else
		return self.tiles[]
	end
end

function Chessboard:traverseLocations(f)
	for x = 1, self.width do
		for y = 1, self.height do
			if f(Location.new(x, y)) then return end
		end
	end
end

function Chessboard:traverseTiles(f)
	self:traverseLocations(function (loc)
		return f(self:getTile(loc))
	end)
end

function Chessboard:route(character, startLoc, targetLoc)
	local source = Astar.newSinglePointSource(startLoc.x, startLoc.y)
	local target = Astar.newSinglePointTarget(targetLoc.x, targetLoc.y)
	self.delegate.routingCharacter = character
	local reversePath = {}
	Astar.route(source, target, self.graph, reversePath)
	
	local locations = {}
	for i=1,#reversePath, 2 do
		locations[math.floor(i/2)+1] = Location.new(reversePath[i], reversePath[i+1])
	end
	
	if #locations > 1 then
		Array.popBack(locations)
	end
	Array.reverse(locations)
	return locations
end

function Chessboard:traverseReachableTiles(character, startLoc, maxcost, f)
	local source = Astar.newSinglePointSource(startLoc.x, startLoc.y)
	local target = Astar.newMaxCostTarget(maxcost)
	self.delegate.routingCharacter = character
	Astar.route(source, target, self.graph)
	
	target:traversePoints(function (x, y)
		return f(self:getTile(Location.new(x, y)))
	end)
end

if select('#', ...) == 0 then 
	local board = Chessboard.new(50, 50)
	local path = board:route(nil, Location.new(1, 1), Location.new(5, 5))
	board:traverseReachableTiles(nil, Location.new(1, 1), 5, function (tile) end)
	board:traverseLocations(function (loc) end)
	board:traverseTiles(function (tile) end)
end

return Chessboard
