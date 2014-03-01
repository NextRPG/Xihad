--- 
-- 处理游戏中使用广度寻路的情景
-- @module PathFinder
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Chessboard = require "Chessboard"
local Queue = require "Queue"

local PathFinder = {}

function PathFinder.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = PathFinder})

	return o
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
function PathFinder:getReachableTiles( character )
	local start, maxAP, manager = character:tile(), character:getProperty("maxAP"), character:getEnemyManager()

	assert(start.x) assert(maxAP)
	self:cleanUp()
	self.data = {}

	start.leftAP = maxAP
	self.start = start 
	local openQueue = Queue.new()
	openQueue:push(start)
	while openQueue:empty() == false do
		local currentPoint = openQueue:pop()
		self.data[hash(currentPoint)] = currentPoint
		if not Chessboard:hasCharacter(currentPoint) then
			self[#self + 1] = currentPoint
			currentPoint.canStay = true
		end
		for k,v in pairs(directions) do

			local tile = Chessboard:tileAt(currentPoint)
			local APcost = tile:getAPCost()
			local point = {x = currentPoint.x + v.x, y = currentPoint.y + v.y, prev = currentPoint, direction = k, leftAP = currentPoint.leftAP - APcost}

			if inbound(point) and Chessboard:tileAt(point):canPass()
				 and point.leftAP > 0 and self.data[hash(point)] == nil 
				 and not manager:getCharacterByLocation(point)  then
				 		openQueue:push(point)
			end
		end
	end
	self[#self + 1] = self.start
	self.start.canStay = true
end

---
-- 通过终点构建路径，路径用direction组成
-- @tab tile
-- @treturn {string, ...} path 
function PathFinder:constructPath( tile )
	local path = {}	
	local tile = self.data[hash(tile)]
	while tile ~= self.start do
		path[#path + 1] = tile.direction
		tile = tile.prev
	end
	table.reverse(path)
	return path
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

function PathFinder:hasTile( tile )
	if self.data[hash(tile)] ~= nil and self.data[hash(tile)].canStay == true then
		return true
	end
	return false
end

return PathFinder
