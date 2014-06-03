local ParticleLoader = {}
local ParticleLoadEnv = require 'Particle.ParticleLoadEnv'

function ParticleLoader.load(file, pnode, source, target, msgReceiver)
	local createMethod = require(file)
	local env = ParticleLoadEnv.new(pnode, source, target, msgReceiver)
	createMethod(pnode, g_particleFactory, env, 1.5)
end

function ParticleLoader.create(file, source, target, msgReceiver)
	local pobject = g_scene:createUniqueObject(c'particleSystem')
	local pnode = pobject:appendComponent(c'ParticleSystem'):getParticleNode()
	ParticleLoader.load(file, pnode:newChild(), source, target, msgReceiver)
	return pobject
end

return ParticleLoader