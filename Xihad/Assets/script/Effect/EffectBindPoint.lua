local ExclusiveEffects = require 'Effect.ExclusiveEffects'
local EffectBindPoint = {
	stickyEffects = nil,
	exclusiveEffects = nil,	-- To avoid duplicated effect
}
EffectBindPoint.__index = EffectBindPoint

function EffectBindPoint.new()
	return setmetatable({
			stickyEffects = {},
			exclusiveEffects = ExclusiveEffects.new(),
		}, EffectBindPoint)
end

function EffectBindPoint:_checkEffectBinding(effect)
	assert(effect:isBound(), 'Invoke effect:bind*() instead')
end

function EffectBindPoint:_checkEffectUnbinding(effect)
	assert(not effect:isBound(), 'Invoke effect:unbind() instead')
end

function EffectBindPoint:registerExclusiveEffect(effect, exclusiveLock)
	self:_checkEffectBinding(effect)
	self.exclusiveEffects:attach(effect, exclusiveLock)
end

function EffectBindPoint:unregisterExclusiveEffect(effect, exclusiveLock)
	self:_checkEffectUnbinding(effect)
	self.exclusiveEffects:detach(effect, exclusiveLock)
end

function EffectBindPoint:registerStickyEffect(effect, source)
	assert(source ~= nil and effect ~= nil)
	assert(self.stickyEffects[effect] == nil, 'register duplicated')
	self:_checkEffectBinding(effect)
	self.stickyEffects[effect] = source
end

function EffectBindPoint:unregisterStickyEffect(effect)
	assert(effect ~= nil)
	self:_checkEffectUnbinding(effect)
	self.stickyEffects[effect] = nil
end

function EffectBindPoint:unbindStickyEffects(source)
	-- remove all
	for effect, theSource in pairs(self.stickyEffects) do
		if theSource == source then
			effect:unbind()
		end
	end
end

return EffectBindPoint
