local ParticleLoader = require 'Particle.ParticleLoader'
local P2PParticleSkillAnimator = {
	particleFile = nil,
}
P2PParticleSkillAnimator.__index = P2PParticleSkillAnimator

function P2PParticleSkillAnimator.new(particleFile)
	local method = require (particleFile)
	assert(type(method) == 'function', 
			string.format('Invalid particle file: %s', particleFile))
	
	return setmetatable({
			particleFile = particleFile,
		}, P2PParticleSkillAnimator)
end

function P2PParticleSkillAnimator:animate(sourceObject, targetTile)
	local warrior = targetTile:getWarrior()
	if not warrior then
		error('This particle effect must be casted to enemy')
	end
	
	local targetObject = warrior:findPeer(c'AnimatedMesh')
	
	local particleSystem = g_scene:createUniqueObject(c'ParticleSystem')
	particleSystem:appendComponent(c'ParticleSystem')
	ParticleLoader.load(self.particleFile, particleSystem, sourceObject, targetObject)
end

return P2PParticleSkillAnimator