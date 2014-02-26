local stack = {

}

function stack.new( o )
	o = o or {}
	setmetatable(o, {__index = stack})

	o.last = 0
	return o
end

function stack:push( v )
	self.last = self.last + 1
	self[self.last] = v
end

function stack:pop(  )
	assert(self.last > 0, "can not pop a empty stack")
	local v = self:top()
	self[self.last] = nil
	self.last = self.last - 1
	return v
end

function stack:top(  )
	assert(self.last > 0, "can not top a empty stack")
	return self[self.last]
end

function stack:empty(  )
	return self.last == 0
end

function stack:clear(  )
	while not self:empty() do
		self:pop()
	end
end

return stack