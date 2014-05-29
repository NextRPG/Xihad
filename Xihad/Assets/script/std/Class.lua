local class = {}

function class.delegate(type, delegated, field, delegating)
	delegating = delegating or delegated
	type[delegated] = function (self, ...)
		local object = self[field]
		if not object then
			error(string.format('no such field to delegate: %s', field))
		end
		
		return object[delegating](object, ...)
	end
end

function class.delegateClosure(type, delegated, object, delegating)
	delegating = delegating or delegated
	if not object[delegating] then
		error(string.format('no such a function in object: %q', delegating))
	end
	
	type[delegated] = function(self, ...)
		return object[delegating](object, ...)
	end
end

function class.copy(object)
	local mt = {}
	if type(object.__ud) == 'userdata' then
		-- This ensures luaT::checkArg<CLASS*>() cast delegate to CLASS*
		setmetatable(mt, getmetatable(object))
	end
	
	-- This ensures delegate copy function and field from object
	mt.__index = object
	
	-- Take all into effect
	return setmetatable({}, mt)
end

if select('#', ...) == 0 then 
	local t1 = {}
	local t2 = {
		bar = function () return 'old' end
	}
	
	class.delegateClosure(t1, 'foo', t2, 'bar')
	assert(t1.foo() == 'old')
	t2.bar = function () return 'new' end
	assert(t1.foo() == 'new')
	
	
end

return class
