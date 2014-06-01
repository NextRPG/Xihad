local BaseCondition = {
	satisfied= false,
	listener = nil,
}
BaseCondition.__index = BaseCondition

function BaseCondition.new(listener)
	return setmetatable({
			listener = listener,
		}, BaseCondition)
end

function BaseCondition:updateCondition()
	error('no implementation by default')
end

function BaseCondition:setSatisfiedListener(listener)
	if self.listener then
		error('Already owns a listener')
	end
	
	self.listener = listener
end

function BaseCondition:getSatisfiedListener()
	return self.listener
end

function BaseCondition:setSatisfied()
	if not self.satisfied then
		self.satisfied = true
		self.listener:onConditionSatisfied(self)
	end
end

function BaseCondition:isSatisfied()
	return self.satisfied
end

return BaseCondition