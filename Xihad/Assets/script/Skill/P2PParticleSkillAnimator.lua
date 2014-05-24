local base = require 'Skill.BaseParticleSkillAnimator'
local P2PParticleSkillAnimator = setmetatable({}, base)
P2PParticleSkillAnimator.__index = P2PParticleSkillAnimator

function P2PParticleSkillAnimator.new(particleFile, magicAnim)
	return setmetatable(base.new(particleFile, magicAnim), P2PParticleSkillAnimator)
end

function P2PParticleSkillAnimator:_getTargetObject(targetTile)
	local targetWarrior = targetTile:getWarrior()
	if not targetWarrior then
		error('This particle effect must be casted to enemy')
	end
	
	return targetWarrior:getHostObject()
end

return P2PParticleSkillAnimator