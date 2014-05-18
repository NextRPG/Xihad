local base = require 'Effect.RoundCounter'
local BoundEffect = setmetatable({
	_binding = nil,
}, base)

BoundEffect.__index = BoundEffect

function BoundEffect.new(round)
	return setmetatable(base.new(round), BoundEffect)
end

function BoundEffect:getBinding()
	return self._binding
end

function BoundEffect:isBound()
	return self._binding ~= nil
end

function BoundEffect:onBind(binding)
end

function BoundEffect:bind(warrior)
	assert(not self:isBound())
	
	self._binding = warrior
	self:onBind(warrior)
end

function BoundEffect:onUnbind()
end

function BoundEffect:unbind()
	if not self._binding then
		error('no binding, please invoke bind() first')
	end
	
	self:onUnbind()
end

function BoundEffect:onExhausted()
	self:unbind()
end

return BoundEffect