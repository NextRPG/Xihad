local loader = {}
local Env = require 'Particle.ParticleLoadEnv'

function loader.load(file, pnode, source, target, messageReceiver)
	local createMethod = require(file)
	local env = Env.new(pnode, source, target, messageReceiver)
	createMethod(pnode, g_particleFactory, env, 1.5)
end

return loader