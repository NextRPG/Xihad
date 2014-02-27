--- 
-- 处理游戏中使用广度寻路的情景
-- @module PathFinder
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Chessboard = require "Chessboard"
local queue = require "Queue"

local PathFinder = {}

function PathFinder.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = PathFinder})

	return o
end

function hash( tile )
	assert(tile.x)
	return tile.x .. ", " .. tile.y
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
	self:cleanUp()

	start.leftAP = maxAP
	self.start = start 
	local openQueue = queue.new()
	openQueue:push(start)
	-- local count = 0
	while openQueue:empty() == false do
		local currentPoint = openQueue:pop()
		self[hash(currentPoint)] = currentPoint
		if not Chessboard:hasCharacter(currentPoint) then
			self[#self + 1] = currentPoint
		end
		for k,v in pairs(directions) do

			local tile = Chessboard:tileAt(currentPoint):findComponent(c"Tile")
			local APcost = tile:getAPCost()
			local point = {x = currentPoint.x + v.x, y = currentPoint.y + v.y, prev = currentPoint, direction = k, leftAP = currentPoint.leftAP - APcost}

			if inbound(point.x, point.y) and Chessboard:tileAt(point):findComponent(c"Tile"):canPass()
				 and point.leftAP > 0 and self[hash(point)] == nil  then
				 		openQueue:push(point)
			end
			-- count = count + 1
		end
	end
	-- print(Chessboard:hasCharacter{x = 9, y = 2} == nil)
	-- for k,v in pairs(self) do
	-- 	if type(v) == "table" and k ~= "start" 
	-- 		-- and tonumber(k) == nil 
	-- 		and not Chessboard:hasCharacter(v) 
	-- 		then
	-- 			print("got tile", v.x, v.y)
	-- 			self[#self + 1] = v
	-- 	end
	-- end
end

---
-- 通过终点构建路径，路径用direction组成
-- @tab tile
-- @treturn {string, ...} path 
function PathFinder:constructPath( tile )
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

function PathFinder:hasTile( tileObject )
	local tile = tileObject:findComponent(c"Tile")
	if self[hash(tile)] ~= nil and not Chessboard:hasCharacter(tile) then
		return true
	end
	return false
end

return PathFinder
