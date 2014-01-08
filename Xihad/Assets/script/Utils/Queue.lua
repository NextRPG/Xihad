local queue = {}

function queue.new( o )
	o = o or {first = 0, last = -1}
	setmetatable(o, {__index = queue})
	return o
end

function queue:push( v )
	self.last = self.last + 1
	self[self.last] = v
end

function queue:pop(  )
	assert(self.first <= self.last, "queue is empty")
	local tmp = self[self.first]
	self[self.first] = nil
	self.first = self.first + 1
	return tmp
end

function queue:empty(  )
	return self.first > self.last
end

return queue
