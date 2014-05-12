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

function P2PParticleSkillAnimator:animate(sourceObject, targetTile, listener)
	local sourceAnimatable = sourceObject:findComponent(c'AnimatedMesh')
	if not sourceAnimatable then
		error('invalid sourceWarrior without AnimatedMesh')
	end
	
	local targetWarrior = targetTile:getWarrior()
	if not targetWarrior then
		error('This particle effect must be casted to enemy')
	end
	
	local targetObject = targetWarrior:getHostObject()
	
	sourceAnimatable:playAnimation(c'magic1', function()
		sourceAnimatable:playAnimation(c'idle')
	end)
	
	local particleObject = g_scene:createUniqueObject(c'ParticleSystem')
	local particleSystem = particleObject:appendComponent(c'ParticleSystem')
	local pnode = particleSystem:getParticleNode()
	ParticleLoader.load(self.particleFile, pnode:newChild(), sourceObject, targetObject)
	
	g_dispatcher:addListener('attack begin', {
			onMessage = function(self, srcObject, param, msgTag)
				if param.source == sourceObject and param.target == targetObject then
					listener:onAttackBegin()
					g_dispatcher:removeListener('attack begin', self)
					self:drop()
				end
			end
		})
	
	g_dispatcher:addListener('attack end', {
			onMessage = function(self, srcObject, param, msgTag)
				listener:onAttackEnd()
				g_dispatcher:removeListener('attack end', self)
				self:drop()
			end
		})
end

return P2PParticleSkillAnimator