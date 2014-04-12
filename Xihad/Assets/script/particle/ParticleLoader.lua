local loader = {}
local Env = require "particle.ParticleLoadEnv"

function loader.load(file, pcomp, source, target)
	local createMethod = require(file)
	local env = Env.new(pcomp, source, target)
	createMethod(pcomp:getParticleNode(), g_particleFactory, env)
end

return loader