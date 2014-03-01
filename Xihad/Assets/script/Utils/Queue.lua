local Queue = {}

function Queue.new( o )
	o = o or {first = 0, last = -1}
	setmetatable(o, {__index = Queue})
	return o
end

function Queue:push( v )
	self.last = self.last + 1
	self[self.last] = v
end

function Queue:pop(  )
	assert(self.first <= self.last, "Queue is empty")
	local tmp = self[self.first]
	self[self.first] = nil
	self.first = self.first + 1
	return tmp
end

function Queue:empty(  )
	return self.first > self.last
end

return Queue
