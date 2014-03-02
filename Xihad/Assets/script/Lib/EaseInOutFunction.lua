local inout = { }

function inout.new(infunc, outfunc)
	local obj = {}
	setmetatable(obj, inout)
	
	obj.infunc = infunc
	obj.outfunc = outfunc
	return obj
end

function inout:__call(p, ex)
	if p < 0.5 then
		return self.infunc(p*2, ex)/2
	else
		return self.outfunc((p-0.5)*2, ex)/2 + 0.5
	end
end

return inout