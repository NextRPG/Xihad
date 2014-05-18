local base = require 'Modifier.Modifier'
local TargetModifier = setmetatable({
	speed = nil,
	target= nil,
	_variable = nil,
	_listeners= {},
}, base)
TargetModifier.__index = TargetModifier

function TargetModifier.new(speed, target, variable)
	assert(variable)
	local o = setmetatable(base.new(), TargetModifier)
	
	o:setSpeed(speed)
	o:setTarget(target)
	o._variable = variable
	
	return o
end

function TargetModifier:addReachListener(lis)
	self._listeners[lis] = true
end

function TargetModifier:removeReachListener(lis)
	self._listeners[lis] = nil
end

function TargetModifier:setSpeed(speed)
	self.speed = math.abs(speed)
end

function TargetModifier:setTarget(target)
	self.target = target
end

function TargetModifier:setLength(delta, length)
	error('No implementation by default')
end

function TargetModifier:between(current, expect, target)
	error('No implementation by default')
end

function TargetModifier:reachTarget()
	return self._variable:get() == self.target
end

function TargetModifier:_fireTargetReached()
	for lis, _ in pairs(self._listeners) do
		lis(self)
	end
end

function TargetModifier:onUpdate(time)
	if self:reachTarget() then 
		self:_fireTargetReached()
		return 'stop'
	end
	
	local current= self._variable:get()
	local delta = self.target - current
	delta = self:setLength(delta, self.speed * time)
	
	local expect = current + delta
	if not self:between(current, expect, self.target) or 
		expect == current then
		expect = self.target
	end
	
	self._variable:set(expect)
end

return TargetModifier