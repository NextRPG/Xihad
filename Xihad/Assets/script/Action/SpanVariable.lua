local span = {
	origin = nil,
	finish = nil,
}
span.__index = span

function span.new(origin, finish)
	local o = {
		origin = origin,
		finish = finish,
	}
	
	setmetatable(o, span)
	return o
end

function span.newWithDelta(origin, delta)
	return span.new(origin, origin+delta)
end

function span:interpolate(d)
	return self.origin*(1-d) + self.finish*d
end

function span:delta()
	return self.finish - self.origin
end

function span:flip()
	self.origin, self.finish = self.finish, self.origin
end

return span
