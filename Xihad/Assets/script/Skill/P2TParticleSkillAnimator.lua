local base = require 'Skill.BaseParticleSkillAnimator'
local P2TParticleSkillAnimator = setmetatable({}, base)
P2TParticleSkillAnimator.__index = P2TParticleSkillAnimator

function P2TParticleSkillAnimator.new(particleFile, magicAnim)
	return setmetatable(base.new(particleFile, magicAnim), P2TParticleSkillAnimator)
end

function P2TParticleSkillAnimator:_getTargetObject(targetTile)
	return targetTile:getTerrain():getHostObject()
end

return P2TParticleSkillAnimator