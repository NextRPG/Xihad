local stack = {

}

function stack.new( o )
	o = o or {top = 0}
	setmetatable(o, {__index = stack})

	return o
end

function stack:push( v )
	self.top = self.top + 1
	self[self.top] = v
end

function stack:pop(  )
	assert(self.top > 0, "can not pop a empty stack")
	local v = self:top()
	self[self.top] = nil
	self.top = self.top - 1
	return v
end

function stack:top(  )
	assert(self.top > 0, "can not top a empty stack")
	return self[self.top]
end

function stack:empty(  )
	return self.top == 0
end

return stack