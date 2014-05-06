local base = require 'Action.Action'

local parallel = {
	finished = false,
	internal_actions = nil,
}
parallel.__index = parallel
setmetatable(parallel, base)

function parallel.new(actions)
	local o = base.new()
	
	assert(#actions >= 1)
	o.internal_actions = actions
	setmetatable(o, parallel)
	return o
end

function parallel:onUpdate(time)
	self.finished = true
	for _, t in ipairs(self.internal_actions) do
		t:update(time)
		self.finished = self.finished and t:hasFinished()
	end
end

function parallel:hasFinished()
	return self.finished
end

function parallel:reset()
	for _, t in ipairs(self.internal_actions) do
		t:reset()
	end
	
	self.allstopped = false
end

return parallel
