require "math3d"
require "mathplus"
require "tableplus"
require "stringplus"
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

function makeList( array, k, v )
	local list = {}
	for i,element in ipairs(array) do
		list[element[k]] = element[v] 		
	end
	return list
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
	if not tile.y then print(debug.traceback()) end
	return tile.x .. " " .. tile.y
end

function inbound( point )
	local x, y = point.x, point.y
	return x >= 0  and x < Consts.COLS and y >= 0 and y < Consts.ROWS 
end

local function search(t, k, plist )
	for i=1, #plist do
		local v = plist[i][k]
		if type(v) == "table" then t[k] = table.copy(v) return t[k]
		elseif v then return v
		end
	end
end 

function inherit( self, ... )
	if getmetatable(self) then
		arg[#arg + 1] = getmetatable(self) 
	end
	setmetatable(self, {__index = function ( t, k )
		return search(t, k, arg)
	end})
	self.__index = self
	return self
end

function g_scene:createObjectWithComponent( name, component, param )
	local object = g_scene:createObject(c(name))
	object:appendComponent(c(component), param)
end

function g_scene:createUniqueObjectWithComponent( component, param )
	local object = g_scene:createUniqueObject(c(component))
	object:appendComponent(c(component), param)
end

function serialize( o )
	print(o)
	if type(o) == "number" then
		io.write(o)
	elseif type(o) == "string" then
		io.write(string.format("%q", o))
	elseif type(o) == "table" then
		io.write(" {\n")
		for k,v in pairs(o) do
			io.write(" ", k, " = ")
			io.write(" [")
			serialize(k)
			io.write("] = ")
			serialize(v)
			io.write(",\n")
		end
		io.write("}")
	else
		error("cannot serialize a " .. type(o))
	end
end

function g_scene:hasObjectWithTag( tag )
	local count = 0
	for object in g_scene:objectsWithTag(tag) do
		count = count + 1
	end
	return count ~= 0
end

function g_scene:getObjectWithTag( tag )
	local result
	for object in g_scene:objectsWithTag(tag) do
		result = object
		break
	end
	return result
end