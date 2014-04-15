local Stack = {

}

function Stack.new( o )
	o = o or {}
	setmetatable(o, {__index = Stack})

	o.last = 0
	return o
end

function Stack:push( v )
	self.last = self.last + 1
	self[self.last] = v
end

function Stack:pop(  )
	assert(self.last > 0, "can not pop a empty Stack")
	local v = self:top()
	self[self.last] = nil
	self.last = self.last - 1
	return v
end

function Stack:top(  )
	assert(self.last > 0, "can not top a empty Stack")
	return self[self.last]
end

function Stack:empty(  )
	return self.last == 0
end

function Stack:clear(  )
	while not self:empty() do
		self:pop()
	end
end

return Stack