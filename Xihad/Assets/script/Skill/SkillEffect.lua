local SkillEffect = {
	damage = 0,
	hitRate= 1.0,
	nature = nil,
}
SkillEffect.__index = SkillEffect

function SkillEffect.new()
	local obj = setmetatable({ }, SkillEffect)
	return obj
end

function SkillEffect:resolve(sourceWarrior, targetWarrior)
	
end

function SkillEffect:resolve()
	
end

return SkillEffect
