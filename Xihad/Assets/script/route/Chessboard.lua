require 'ArtIn'

local Array = require 'std.Array'
local MapTile  = require 'route.MapTile'
local Location = require 'route.Location'

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
	local b = setmetatable({
		width = width,
		height= height,
		tiles = {},
	}, Chessboard)
	
	b.delegate = {
		computeCost = function(self, x1, y1, x2, y2)
			local tile = b:getTile(Location.new(x2, y2))
			if not tile:canPass(self.routingWarrior) then
				return -1	-- Negative cost edge will be discarded
			end
			
			local cost = tile:getActionPointCost(self.routingWarrior)
			assert(cost >= 1)
			return cost
		end
	}
	b.graph = Astar.newChessboard(b.width, b.height, b.delegate)
	
	mapTileFactory = mapTileFactory or MapTile.new
	b:traverseLocations(function (loc)
		b.tiles[toIndex(loc, b.width)] = mapTileFactory(loc)
	end)
	return b
end

function Chessboard:getTile(location)
	if location.x <= 0 or location.y <= 0 or location.x > self.width then
		return nil
	end
	
	local index = toIndex(location, self.width)
	
	if index <= 0 or index > #self.tiles then
		return nil
	end
	
	-- Finally
	return self.tiles[index]
end

function Chessboard:traverseLocations(f)
	for x = 1, self.width do
		for y = 1, self.height do
			if f(Location.new(x, y)) then return end
		end
	end
end

function Chessboard:_getSource(warrior, startLoc)
	startLoc = startLoc or warrior:getLocation()
	return Astar.newSinglePointSource(startLoc.x, startLoc.y)
end

function Chessboard:_transformPath(revPath)
	local locations = {}
	for i = 1, #revPath, 2 do
		locations[math.floor(i/2)+1] = Location.new(revPath[i], revPath[i+1])
	end
	
	if #locations > 1 then
		Array.popBack(locations)	-- pop start location
	end
	
	return locations
end

function Chessboard:_route(warrior, targetLoc, startLoc)
	local source = self:_getSource(warrior, startLoc)
	local target = Astar.newSinglePointTarget(targetLoc.x, targetLoc.y)
	self.delegate.routingWarrior = warrior
	local revXY = {}
	local cost = Astar.route(source, target, self.graph, revXY)
	
	if #revXY > 0 then return revXY, cost end
end

---
-- if startLoc == targetLoc, return { targetLoc }
-- if startLoc ~= targetLoc, return path without startLoc or nil
function Chessboard:routeRev(warrior, targetLoc, startLoc)
	local revXY, cost = self:_route(warrior, targetLoc, startLoc)
	if revXY then
		return self:_transformPath(revXY), cost
	end
end

function Chessboard:route(warrior, targetLoc, startLoc)
	local revPath, cost = self:routeRev(warrior, targetLoc, startLoc)
	if revPath then
		return Array.reverse(revPath), cost
	end
end

function Chessboard:canReach(warrior, targetLoc, startLoc)
	local revXY, cost = self:_route(warrior, targetLoc, startLoc)
	return revXY ~= nil and cost <= warrior:getActionPoint()
end

function Chessboard:getReachableTiles(warrior, startLoc)
	local source = self:_getSource(warrior, startLoc)
	local target = Astar.newMaxCostTarget(warrior:getActionPoint())
	self.delegate.routingWarrior = warrior
	Astar.route(source, target, self.graph)
	
	local allTiles = {}
	target:traversePoints(function (x, y)
		local tile = self:getTile(Location.new(x, y))
		if tile:canStay(warrior) then
			return table.insert(allTiles, tile)
		end
	end)
	
	return allTiles
end

if select('#', ...) == 0 then 
	local board = Chessboard.new(50, 50)
	local path = board:route(nil, Location.new(1, 1), Location.new(5, 5))
	local warrior = { 
		getActionPoint = function ()
			return 5
		end,
		getLocation = function ()
			return Location.new(1, 1)
		end
	}
	
	for _, tile in ipairs(board:getReachableTiles(warrior)) do
		
	end
	
	board:traverseLocations(function (loc) end)
	board:traverseTiles(function (tile) end)
end

return Chessboard
