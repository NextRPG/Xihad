local out = {}

function out.new(infunc)
	assert(infunc)
	
	local obj = {}
	setmetatable(obj, out)
	
	obj.infunc = infunc
	return obj
end

function out:__call(p, ex)
	return 1 - self.infunc(1-p, ex)
end

return out
