-- function findMax( list )
-- 	local maxKey, maxValue = next(list)
-- 	for k,v in pairs(list) do
-- 		if maxValue < v then
-- 			maxKey, maxValue = k, v
-- 		end	
-- 	end
-- 	return maxKey, maxValue
-- end

-- function findMin( list )
-- 	local minKey, minValue = next(list)
-- 	for k,v in pairs(list) do
-- 		if minValue > v then
-- 			minKey, minValue = k, v
-- 		end
-- 	end
-- 	return minKey, minValue
-- end
-- list = { a = 1, b = 2}
-- print(findMin(list))
-- print(findMax(list))

-- function hex2Color( str )
-- 	return tonumber("0x" .. str:sub(1, 2)), tonumber("0x" .. str:sub(3, 4)), tonumber("0x" .. str:sub(5, 6))
-- end

-- print((1 == 2) and "haha" or "hehe")

-- create a namespace

package.path = package.path 
.. ";../Utils/?.lua"

require "tableplus"
local Publisher = require "Publisher"

local function search(t, k, plist )
	for i=1, plist.n do
		local v = plist[i][k]
		if type(v) == "table" then t[k] = table.copy(v) return t[k]
		elseif v then return v
		end
	end
end 

function inherit( self, ... )
	setmetatable(self, {__index = function ( t, k )
		return search(t, k, arg)
	end})
	self.__index = self
	return self
end

local Character = {HP = 100}

function Character.new( o )
	o = o or {}
	inherit(o, Publisher, Character)
	return o
end

function Character:onHPchanged( HP )
	print("HP is " .. HP)
end

local Tom = Character.new{}
local John = Character.new{}
-- Tom:attachSubscriber("HP", John, John.onHPchanged)
-- Tom:changeState("HP", 50)
for k,v in pairs(Tom) do
	print(k,v)
end

-- Tom.subscribers[1] = 5
-- for k,v in pairs(Tom.subscribers) do
-- 	print(k,v)
-- end
