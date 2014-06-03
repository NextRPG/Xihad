local BaseChecker = {
	_listener = nil,
	conditions= nil,
}
BaseChecker.__index = BaseChecker

function BaseChecker.new(listener)
	return setmetatable({
			_listener = listener,
			conditions= {},
		}, BaseChecker)
end

function BaseChecker:addCondition(condition, userdata)
	assert(userdata ~= nil)
	
	condition:setSatisfiedListener(self)
	self.conditions[condition] = userdata
end

function BaseChecker:removeCondition(condition)
	condition:setSatisfiedListener(nil)
	self.conditions[condition] = nil
end

function BaseChecker:onConditionSatisfied(condition)
	local userdata = self.conditions[condition]
	if userdata == nil then 
		error("Checker shouldn't be notified by other conditions")
	end
	
	self:removeCondition(condition)
	self._listener:onChecked(userdata)
end

function BaseChecker:onCheckPoint()
	for condition, _ in pairs(self.conditions) do
		assert(condition:getSatisfiedListener() == self)
		condition:checkCondition()
	end
end


return BaseChecker