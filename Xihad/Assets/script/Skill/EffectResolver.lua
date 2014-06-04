local base = require 'Skill.ToComponentResolver'
local EffectResolver = {
	boundEffect = nil,
	probability = nil,
}
EffectResolver.__index = EffectResolver
setmetatable(EffectResolver, base)

function EffectResolver.new(boundEffect, probability)
	local obj = setmetatable(base.new('Warrior'), EffectResolver)
	
	boundEffect.__index = boundEffect
	obj.boundEffect = boundEffect
	obj.probability = probability
	return obj
end

function EffectResolver:_resolve(sourceWarrior, targetWarrior, relativeLoc, result)
	result:addSkillEffect(self.boundEffect, self.probability)
end

return EffectResolver
