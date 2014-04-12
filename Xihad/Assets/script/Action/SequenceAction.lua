local Action = require 'Action.Action'

local sequence = {
	idle	 = 0,
	iterator = 1,
	internal_actions = nil,
}
sequence.__index = sequence
setmetatable(sequence, Action)

function sequence.new(actions)
	local o = Action.new()
	o.internal_actions = actions
	setmetatable(o, sequence)
	return o
end

function sequence:onUpdate(time)
	local current = self.internal_actions[self.iterator]
	local finish
	if type(current) == 'number' then
		self.idle = self.idle + time
		finish = self.idle >= current
	else
		current:update(time)
		finish = current:hasFinished()
	end
	
	if finish then
		self.iterator = self.iterator + 1
		self.idle = 0
	end
end

function sequence:hasFinished()
	return self.iterator > #self.internal_actions
end

function sequence:reset()
	for i = 1, self.iterator do
		if type(self.internal_actions[i]) ~= 'number' then
			self.internal_actions[i]:reset()
		end
	end
	
	self.iterator = 1
end

return sequence
