local base = require 'Skill.ToWarriorResolver'
local BuffResolver = {
	boundEffect = nil,
	probability = nil,
}
BuffResolver.__index = BuffResolver
setmetatable(BuffResolver, base)

function BuffResolver.new(boundEffect, probability)
	local obj = setmetatable(base.new(), BuffResolver)
	
	boundEffect.__index = boundEffect
	obj.boundEffect = boundEffect
	obj.probability = probability
	return obj
end

function BuffResolver:_resolve(sourceWarrior, targetWarrior, relativeLoc, result)
	local copyEffect = setmetatable({}, self.boundEffect)
	result:addSkillEffect(copyEffect, self.probability)
end

return BuffResolver
