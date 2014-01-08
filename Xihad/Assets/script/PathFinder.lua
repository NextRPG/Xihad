--- 
-- 处理游戏中有关寻路的情景
-- @module name
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Chessboard = require "Chessboard"
local queue = require "Queue"

local PathFinder = {}

function hash( tile )
	assert(tile.x)
	return tile.x .. " " .. tile.y
end

function inbound( x, y )
	return x >= 0  and x < Consts.COLS and y >= 0 and y < Consts.ROWS 
end

-- 这种遍历查找重复的方法没有哈希表的方法快，但是也有好处(其实也没什么好处)
function findTile( location )
	local testvalue = hash(location)
	for k,v in pairs(PathFinder) do
		if k == testvalue then
			return v
		end
	end
	return nil
end

---
-- 通过起始点和最大AP计算出所有可能到达的节点.不返回值
-- PathFinder本身已经包含了所有可以到达的tile的坐标
-- @tab start
-- @int maxAP
local directions = Consts.directions
function PathFinder:getReachableTiles( start, maxAP, predicate )
	assert(start) assert(maxAP)
	assert(self.start == nil, "must clean before getReachableTiles")
	start.leftAP = maxAP
	self.start = start 
	self[hash(start)] = start
	local openQueue = queue.new()
	openQueue:push(start)
	while openQueue:empty() == false do
		currentPoint = openQueue:pop()
		self[hash(currentPoint)] = currentPoint
		for k,v in pairs(directions) do

			local tile = Chessboard:tileAt(currentPoint):findComponent(c"Tile")
			local APcost = tile:getAPCost()
			local point = {x = currentPoint.x + v.x, y = currentPoint.y + v.y, prev = currentPoint, direction = k, leftAP = currentPoint.leftAP - APcost}


			if inbound(point.x, point.y) and Chessboard:tileAt(point):findComponent(c"Tile"):canPass()
				 and point.leftAP > 0 and self[hash(point)] == nil  then
				 	if not predicate or predicate(point) then
				 		openQueue:push(point)
					end
			end
		end
	end
	assert(self.start)
	for k,v in pairs(self) do
		if type(v) == "table" then
			self[#self + 1] = v
		end
	end
end

--- 
-- 获得到达所有敌人处的最短路径
-- @tab start
-- @tparam CharacterManager manager
local fakeMaxAP = 10000
function PathFinder:getEnemyPath( start, manager )
	local fakeMaxAP = 10000
	local predicate = function ( point )
		return manager:getHeroByLocation(point)
	end
	self:getReachableTiles(start, fakeMaxAP, predicate)
end

---
-- 获得到达所有敌人的路径后，找到一个最能走到范围内的目标点
function PathFinder:getEnemyTile( finalTile, maxAP )
	while fakeMaxAP - finalTile.prev.leftAP > maxAP do
		finalTile = finalTile.prev
	end
	return finalTile.prev
end

---
-- 通过终点构建路径，路径用direction组成
-- @tab tile
-- @treturn {string, ...} path 
function PathFinder:constructPath( tile )
	local path = {}
	local tile = findTile(tile)
	assert(tile)
	while tile ~= self.start do
		path[#path + 1] = tile.direction
		tile = tile.prev
	end
	table.reverse(path)
	return path
end

---
-- 构建路径后在将缓存清理
-- @tab tile
-- @treturn {string, ...} path 
function PathFinder:constructPathAndClean( tile )
	local tmp = self:constructPath( tile )
	self:cleanUp()
	return tmp
end

---
-- 清理缓存
function PathFinder:cleanUp(  )
	-- 一个教训，迭代器返回的是值而不是引用
	for k,v in pairs(self) do
		if (type(v) == "table") then
			self[k] = nil
		end
	end
end

return PathFinder
