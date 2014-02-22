local class = {}

function class.def(clazz, base, option)
	clazz = clazz or {}
	clazz.__index = clazz
	
	if base then
		assert(type(base) == "table")
		setmetatable(clazz, base)
	end
	
	if option == "plain_new" then
		function clazz.new()
			return class.new(clazz)
		end
	end
	
	return clazz
end

function class.new(clazz, o)
	o = o or {}
	setmetatable(o, clazz)
	return o
end

function class.accessor(clazz, attrib, access)
	access = access or "readwrite"
	assert(type(attrib) == "string")
	assert(access=="readonly" or access=="readwrite")
	
	local Attrib = string.upper(attrib:sub(1, 1))..attrib:sub(2)
	clazz["get"..Attrib] = function (o)
		return o[attrib]
	end
	
	if access == "readwrite" then
		clazz["set"..Attrib] = function (o, v)
			o[attrib] = v
		end
	end
end

if select('#', ...) == 0 then 
	-- no base class and has plain new function
	local t = class.def({ value = 10 }, nil, "plain_new")
	
	-- generate function getValue() and setValue()
	class.accessor(t, "value")
	
	local to = t.new()
	assert(to:getValue() == 10)
	to:setValue(2)
	assert(to:getValue() == 2)
end

return class