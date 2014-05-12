---
-- Modifier is a update handler which has no explicit finish point.
-- But it can be disabled at any time.
local Modifier = {
	_enabled = nil,
}
Modifier.__index = Modifier

function Modifier.new()
	return setmetatable({}, Modifier)
end

function Modifier:start()
	if self._enabled ~= nil then
		return
	end
	
	self:onStart()
	self._enabled = false
	self:setEnabled(true)
	
	assert(self._enabled ~= nil)
end

function Modifier:setEnabled(b)
	if self._enabled == nil then
		self:start()
	end
		
	if self._enabled ~= b then
		assert(type(b) == 'boolean')
		self._enabled = b
		
		if b then
			self:onResume()
		else
			self:onPause()
		end
	end
end

function Modifier:onStart()
end

function Modifier:onResume()
end

function Modifier:onPause()
end

function Modifier:isEnabled()
	return self._enabled
end

function Modifier:onUpdate(time)
	error('No implementation by default')
end

function Modifier:update(time)
	if self._enabled then
		self:onUpdate(time)
	elseif self._enabled == nil then
		self:start()
		self:update(time)
	end
end

return Modifier