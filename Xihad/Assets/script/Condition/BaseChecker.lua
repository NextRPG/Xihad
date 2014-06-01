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
		return false
	end
	
	if not self._listener:onChecked(userdata) then
		return false
	end
	
	return true
end

function BaseChecker:onCheckPoint()
	for condition, _ in pairs(self.conditions) do
		assert(condition:getSatisfiedListener() == self)
		condition:updateCondition()
	end
	
	return false
end


return BaseChecker