--- 
-- 处理游戏中使用A星算法寻路的场景
-- @module GoalFinder
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Chessboard = require "Chessboard"

local GoalFinder = {}

function hash( tile )
	assert(tile.x)
	return tile.x .. " " .. tile.y
end

function inbound( x, y )
	return x >= 0  and x < Consts.COLS and y >= 0 and y < Consts.ROWS 
end

local function findMin( list, scores )
	local point, minScore = {}, 100000
	for k,v in pairs(list) do
		if scores[k] < minScore then
			point, minScore = v, scores[k]
		end
	end
	return point, minScore
end

function GoalFinder:Astar( start, goal, maxAP )
	assert(start) assert(goal)
	self:cleanUp()
	start.leftAP = maxAP
	self.start = start 
	local openSet, g_score, h_score, f_score  = {}, {}, {}, {}
	openSet[hash(start)] = start

	g_score[hash(start)] = 0
	h_score[hash(start)] = math.abs(goal.y - start.y) + math.abs(goal.x - start.x)
 	f_score[hash(start)] = h_score[hash(start)]

	while next(openSet) ~= nil do
		local currentPoint = findMin(openSet, f_score)
		if hash(currentPoint) == hash(goal) then
			return true
		end
		openSet[hash(currentPoint)] = nil
		self[hash(currentPoint)] = currentPoint
		
		for k,v in pairs(directions) do
			repeat 
			local tile = Chessboard:tileAt(currentPoint):findComponent(c"Tile")
			local APcost = tile:getAPCost()
			local point = {x = currentPoint.x + v.x, y = currentPoint.y + v.y, prev = currentPoint, direction = k, leftAP = currentPoint.leftAP - APcost}

			if self[hash(point)] ~= nil or not inbound(point) then
				break
			end
			tentative_g_score = g_score[hash(point)] + 1

			local tentative_is_better = true
			if openSet[hash(point)] == nil or tentative_g_score < g_score[hash(point)] then
				openSet[hash(point)] = point
				g_score[hash(point)] = tentative_g_score
				h_score[hash(point)] = math.abs(goal.y - point.y) + math.abs(goal.x - point.x)
			 	f_score[hash(start)] = h_score[hash(start)] + g_score[hash(point)]
			end
			until true
		end
	end
	error("not a connected graph")
end

local fakeMaxAP = 100000
function GoalFinder:getCostAP( start, goal )
	self:Astar(start, goal, fakeMaxAP)
	return self[hash(goal)].leftAP
end

function GoalFinder:getTargetTile( start, goal, maxAP )
	self:Astar(start, goal)
	local tile = self[hash(goal)]
	while fakeMaxAP - tile.prev.leftAP > maxAP do
		tile = tile.prev
	end
	return tile.prev
end

function GoalFinder:constructPath( tile )
	local path = {}
	local tile = self[hash(tile)]
	assert(tile)
	while tile ~= self.start do
		path[#path + 1] = tile.direction
		tile = tile.prev
	end
	table.reverse(path)
	return path
end

function GoalFinder:cleanUp(  )
	-- 一个教训，迭代器返回的是值而不是引用
	for k,v in pairs(self) do
		if (type(v) == "table") then
			self[k] = nil
		end
	end
end