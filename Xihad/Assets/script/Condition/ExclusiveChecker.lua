local base = require 'Condition.BaseChecker'
local ExclusiveChecker = setmetatable({}, base)
ExclusiveChecker.__index = ExclusiveChecker

function ExclusiveChecker.new(listener)
	return setmetatable(base.new(listener), ExclusiveChecker)
end

function ExclusiveChecker:onConditionSatisfied(condition)
	if self._listener then
		base.onConditionSatisfied(self, condition)
		self._listener = nil
	end
end

function ExclusiveChecker:onCheckPoint(context)
	if not self._listener then
		return
	end
	
	for condition, _ in pairs(self.conditions) do
		assert(condition:getSatisfiedListener() == self)
		condition:checkCondition(context)
		if condition:isSatisfied() then
			assert(self._listener == nil)
			return
		end
	end
end

return ExclusiveChecker