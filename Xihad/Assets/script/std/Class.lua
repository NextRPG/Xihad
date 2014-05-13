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
	type[delegated] = function(self, ...)
		return object[delegating](object, ...)
	end
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
