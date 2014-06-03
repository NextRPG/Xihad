local ParticleLoader = require 'Particle.ParticleLoader'
local BaseParticleSkillAnimator = {
	particleFile = nil,
	magicAnim = 'magic1',
}
BaseParticleSkillAnimator.__index = BaseParticleSkillAnimator

function BaseParticleSkillAnimator.new(particleFile, magicAnim)
	-- local method = require (particleFile)
	-- assert(type(method) == 'function', 
	-- 		string.format('Invalid particle file: %s', particleFile))
	return setmetatable({
			particleFile = particleFile,
			magicAnim = magicAnim,
		}, BaseParticleSkillAnimator)
end

function BaseParticleSkillAnimator:_getTargetObject(targetTile)
	return nil
end

function BaseParticleSkillAnimator:_animateSource(sourceObject)
	local sourceAnimatable = sourceObject:findComponent(c'AnimatedMesh')
	if not sourceAnimatable then
		error('invalid sourceWarrior without AnimatedMesh')
	end
	
	sourceAnimatable:playAnimation(c(self.magicAnim), function()
		sourceAnimatable:playAnimation(c'idle')
	end)
end

function BaseParticleSkillAnimator:animate(sourceObject, targetTile, listener)
	self:_animateSource(sourceObject)
	local targetObject = self:_getTargetObject(targetTile)
	
	local particleObject = 
		ParticleLoader.create(self.particleFile, sourceObject, targetObject, 
			function (message)
				if message == 'attack begin' then
					listener:onAttackBegin()
				elseif message == 'attack end' then
					listener:onAttackEnd()
				elseif message == 'destroy' then
					particleObject:stop()
				end
			end)
end

return BaseParticleSkillAnimator