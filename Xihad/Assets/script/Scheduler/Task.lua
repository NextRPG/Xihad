local Task = {
	taskId = nil,
	callback = nil,
	startTime= nil,
}
Task.__index = Task

function Task.new(id, startTime, callback)
	assert(callback)
	
	return setmetatable({
			taskId = id,
			callback = callback,
			startTime= startTime,
		}, Task)
end

function Task:__lt(other)
	if self.startTime < other.startTime then
		return true
	elseif self.startTime > other.startTime then
		return false
	else
		return self.taskId < other.taskId
	end
end

function Task:__gt(other)
	return not self:__lt(other)
end

function Task:hasExpired(current)
	return self.startTime <= current
end

function Task:hasStarted()
	return self.callback == nil
end

function Task:start()
	assert(not self:hasStarted())
	local run = self.callback
	self.callback = nil
	run()
end

function Task:_resetTime(startTime)
	self.startTime = startTime
end

return Task