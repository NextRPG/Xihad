local InterruptableAction = {
	_enabled = true,
}
InterruptableAction.__index = InterruptableAction

function InterruptableAction.new()
	return setmetatable({}, InterruptableAction)
end

function InterruptableAction:setEnabled(b)
	assert(type(b) == 'boolean')
	self._enabled = b
end

function InterruptableAction:isEnabled()
	return self._enabled
end

function InterruptableAction:onUpdate(time)
	error('No implementation by default')
end

function InterruptableAction:update(time)
	if self._enabled then
		return self:onUpdate(time)
	end
end

return InterruptableAction