local CompositeEffect = {
	effects = nil
}
CompositeEffect.__index = CompositeEffect

function CompositeEffect.new()
	return setmetatable({
			effects = {}
		}, CompositeEffect)
end

function CompositeEffect:addEffect(effect)
	assert(effect)
	table.insert(self.effects, effect)
end

function CompositeEffect:copy()
	local copy = CompositeEffect.new()
	for _, effect in ipairs(self.effects) do
		copy:addEffect(effect:copy())
	end
	
	return copy
end

function CompositeEffect:getBinding()
	if next(self.effects) ~= nil then
		return self.effects[1]:getBinding()
	end
end

function CompositeEffect:bindSticky(bindPoint, source)
	for _, effect in ipairs(self.effects) do
		effect:bindSticky(bindPoint, source)
	end
end

function CompositeEffect:bindExclusive(bindPoint, cause)
	for _, effect in ipairs(self.effects) do
		effect:bindExclusive(bindPoint, cause)
	end
end

function CompositeEffect:unbind()
	for _, effect in ipairs(self.effects) do
		effect:unbind()
	end
end

return CompositeEffect