local base = require 'Condition.BaseChecker'
local ExclusiveChecker = setmetatable({}, base)
ExclusiveChecker.__index = ExclusiveChecker

function ExclusiveChecker.new(listener)
	assert(listener ~= nil)
	return setmetatable(base.new(listener), ExclusiveChecker)
end

function ExclusiveChecker:onConditionSatisfied(condition)
	if self._listener and base.onConditionSatisfied(self, condition) then
		self._listener = nil -- For exclusive checker, just set it to nil
		return true
	end
	
	return false
end

---
-- Return if we should action more
function ExclusiveChecker:onCheckPoint()
	if not self._listener then
		return true
	end
	
	for condition, _ in pairs(self.conditions) do
		assert(condition:getSatisfiedListener() == self)
		condition:updateCondition()
		if condition:isSatisfied() then
			assert(self._listener == nil)
			return true -- No need to check the rest for exclusive checker
		end
	end
	
	return false
end

return ExclusiveChecker