local loader = {}
local Env = require 'Particle.ParticleLoadEnv'

function loader.load(file, pnode, source, target)
	local createMethod = require(file)
	local env = Env.new(pnode, source, target)
	createMethod(pnode, g_particleFactory, env)
end

return loader