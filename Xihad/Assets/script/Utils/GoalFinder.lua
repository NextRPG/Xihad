--- 
-- 处理游戏中使用A星算法寻路的场景
-- @module GoalFinder
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Chessboard = require "Chessboard"
local PathFinder = require "PathFinder"

local GoalFinder = PathFinder.new{}

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
function GoalFinder:Astar( start, goal )

	assert(start) assert(goal)
	self:cleanUp()
	start.leftAP = fakeMaxAP -- fake
	self.start = start 
	local openSet, g_score, h_score, f_score  = {}, {}, {}, {}
	openSet[hash(start)] = start

	g_score[hash(start)] = 0
	h_score[hash(start)] = math.abs(goal.y - start.y) + math.abs(goal.x - start.x)
 	f_score[hash(start)] = h_score[hash(start)]

	while next(openSet) ~= nil do
		local currentPoint = findMin(openSet, f_score)
		openSet[hash(currentPoint)] = nil
		self[hash(currentPoint)] = currentPoint
		if hash(currentPoint) == hash(goal) then
			return true
		end
		
		for k,v in pairs(directions) do

			repeat 
			local tile = Chessboard:tileAt(currentPoint)
			local APcost = tile:getAPCost()
			local point = {x = currentPoint.x + v.x, y = currentPoint.y + v.y, prev = currentPoint, direction = k, leftAP = currentPoint.leftAP - APcost}

			if self[hash(point)] ~= nil or not inbound(point.x, point.y) or not Chessboard:tileAt(point):canPass() then
				break
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
	error("not a connected graph")
end

function GoalFinder:getCostAP( start, goal )
	self:Astar(start, goal)
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

return GoalFinder