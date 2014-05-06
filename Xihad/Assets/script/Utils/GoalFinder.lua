--- 
-- 处理游戏中使用A星算法寻路的场景
-- @module GoalFinder
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Chessboard = require "ColoringManager"
local AIManager = require "AIManager"
local HeroManager = require "HeroManager"

local GoalFinder = {}

local function findMin( list, scores )
	local point, minScore = {}, 100000
	for k,v in pairs(list) do
		if scores[k] < minScore then
			point, minScore = v, scores[k]
		end
	end
	return point, minScore
end

local fakeMaxAP = 100000
local directions = Consts.directions
function GoalFinder:Astar( start, goal, maxAP, predicate )
	assert(start) assert(goal)
	-- print(type(self.data[hash{x = 6, y = 2}]))
	-- self:cleanUp()
	self.data = {}
	-- print(type(self.data[hash{x = 6, y = 2}]))
	self.start = start 

	start.leftAP = fakeMaxAP -- fake
	local openSet, g_score, h_score, f_score  = {}, {}, {}, {}
	openSet[hash(start)] = start

	g_score[hash(start)] = 0
	h_score[hash(start)] = math.abs(goal.y - start.y) + math.abs(goal.x - start.x)
 	f_score[hash(start)] = h_score[hash(start)]

	while next(openSet) ~= nil do
		local currentPoint = findMin(openSet, f_score)
		openSet[hash(currentPoint)] = nil
		self.data[hash(currentPoint)] = currentPoint
		if hash(currentPoint) == hash(goal) then
			return true
		end
		
		for k,v in pairs(directions) do
		repeat 
			local tile = Chessboard:tileAt(currentPoint)
			local APcost = tile:getActionPointCost()
			local point = {x = currentPoint.x + v.x, y = currentPoint.y + v.y, prev = currentPoint, direction = k, leftAP = currentPoint.leftAP - APcost}


			if self.data[hash(point)] ~= nil or not inbound(point)
			 or not Chessboard:tileAt(point):canPass() 
			 or (HeroManager:getCharacterByLocation(point) and not math.p_same(point, goal))
			 or (predicate and predicate(currentPoint, point)) 
			 then
				break	-- simulate continue
			end
			

			local tentative_g_score = g_score[hash(currentPoint)] + 1

			if openSet[hash(point)] == nil or tentative_g_score < g_score[hash(point)] then
				
				openSet[hash(point)] = point
				g_score[hash(point)] = tentative_g_score
				h_score[hash(point)] = math.abs(goal.y - point.y) + math.abs(goal.x - point.x)
			 	f_score[hash(point)] = h_score[hash(point)] + g_score[hash(point)]
			end
		until true
		end
	end
	
	return false
end


---
-- 通过终点构建路径，路径用direction组成
-- @tab tile
-- @treturn {string, ...} path 
function GoalFinder:constructPath( tile )
	local path = {}	
	local tile = self.data[hash(tile:getLocation())]
	while tile ~= self.start do
		path[#path + 1] = tile.direction
		tile = tile.prev
	end
	table.reverse(path)
	return path
end

function GoalFinder:getCostAP( start, goal, maxAP )
	if self:Astar(start, goal, maxAP) then
		return self.data[hash(goal)].leftAP
	else
		return "MAX"
	end
end

function GoalFinder:findPrevious( start, goal, maxAP)
	local tile = self.data[hash(goal)]
	while fakeMaxAP - tile.prev.leftAP > maxAP do
		tile = tile.prev
	end
	while tile.prev ~= start 
		and AIManager:getCharacterByLocation(tile.prev) do
		tile = tile.prev
	end
	return tile.prev
end

function GoalFinder:getTargetTile( start, goal, maxAP )
	if	self:Astar(start, goal, maxAP, 
		function ( currentPoint, point ) 
			return AIManager:getCharacterByLocation( currentPoint ) and math.p_same(point, goal)
		end) then
		return self:findPrevious(start, goal, maxAP)
	else
		self:Astar(start, goal, maxAP)
		return self:findPrevious(start, goal, maxAP)
	end
end

function GoalFinder:getTargetTileRemote( start, goal, maxAP, minDistance )
	local tile = self:getTargetTile(start, goal, maxAP)
	
	while tile ~= start do
		if (not AIManager:getCharacterByLocation(tile) and
			math.p_distance(goal, tile) >= minDistance) 
		then
			return tile
		end
		
		tile = tile.prev
	end
	
	return tile
end

function GoalFinder:cleanUp(  )
	-- 一个教训，迭代器返回的是值而不是引用
	for k,v in pairs(self) do
		if (type(v) == "table") then
			self[k] = nil
			print(k, type(self[k]))
		end
	end
end

return GoalFinder