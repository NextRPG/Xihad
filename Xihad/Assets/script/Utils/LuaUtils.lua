require "math3d"
require "mathplus"
require "tableplus"
require "coordinate"

-- function

function runCallback( result, func, t )
	if type(func) == "function" then		
		func(unpack(t))
	end
	if result ~= true then print(func) end
end

function runAsyncFunc( func, ... )

	local current = coroutine.running()
	local t = {...}
	table.insert(t, function (  )
		runCallback(coroutine.resume(current))
	end)
	
	coroutine.yield(func, t)
end

function findMax( list )
	local maxKey, maxValue = next(list)
	for k,v in pairs(list) do
		if maxValue < v then
			maxKey, maxValue = k, v
		end	
	end
	return maxKey, maxValue
end

function findMin( list )
	local minKey, minValue = next(list)
	for k,v in pairs(list) do
		if minValue > v then
			minKey, minValue = k, v
		end
	end
	return minKey, minValue
end

function hex2Color( str )
	return tonumber("0x" .. str:sub(1, 2)), tonumber("0x" .. str:sub(3, 4)), tonumber("0x" .. str:sub(5, 6))
end

function hash( tile )
	assert(tile.x)
	return tile.x .. " " .. tile.y
end

function inbound( point )
	local x, y = point.x, point.y
	return x >= 0  and x < Consts.COLS and y >= 0 and y < Consts.ROWS 
end

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