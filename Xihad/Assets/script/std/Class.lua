local class = {}

function class.delegate(type, delegated, field, delegating)
	delegating = delegating or delegated
	type[delegated] = function (self, ...)
		local object = self[field]
		return object[delegating](object, ...)
	end
end

function class.delegateClosure(type, delegated, object, delegating)
	type[delegated] = function(self, ...)
		return object[delegating](object, ...)
	end
end

return class
