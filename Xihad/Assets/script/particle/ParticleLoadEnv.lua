local ParticleLoadEnv = { 
	pobject= nil,
	source = nil, 
	target = nil, 
	defaultScale = 1.5,
	messageReceiver = nil,
}
ParticleLoadEnv.__index = ParticleLoadEnv

function ParticleLoadEnv.new(source, target, msgReceiver)
	local env = setmetatable({
			source = source,
			target = target,
			messageReceiver = msgReceiver,
		}, ParticleLoadEnv)
	
	env.pobject = env:_createParticleObject()
	env:_check_psystem()
	return env
end

function ParticleLoadEnv.newSingle(source, msgReceiver)
	return ParticleLoadEnv.new(source, nil, msgReceiver)
end

function ParticleLoadEnv:_createParticleObject()
	local pobject = g_scene:createUniqueObject(c'particle_object')
	pobject:appendComponent(c'ParticleSystem')
	return pobject
end

function ParticleLoadEnv:_check_psystem()
	return assert(self.pobject:findComponent(c'ParticleSystem'))
end

function ParticleLoadEnv:_create_new_node()
	local system = self:_check_psystem()
	return system:getParticleNode():newChild()
end

function ParticleLoadEnv:inflate(file, scale)
	local createMethod = require (file)
	if type(createMethod) ~= 'function' then
		error('Not valid particle file')
	end
	
	scale = scale or self.defaultScale
	createMethod(self:_create_new_node(), g_particleFactory, self, scale)
end

function ParticleLoadEnv:getParticleObject()
	return self.pobject
end

------------------------------------------------------------------------------
--- Particle System Envieonment Interface
function ParticleLoadEnv:getNode(nodeDesc)
	if nodeDesc:sub(1, 1) == '@' then
		local t = nodeDesc:sub(2)
		if t == 'source' or t == 'target' then
			return self[t]
		end
	end
end

function ParticleLoadEnv:getPosition(obj)
	return obj:getTranslate()
end

local function getRender(obj)
	return obj:findComponent(c'Render')
end

function ParticleLoadEnv:getAABB(obj)
	return getRender(obj):getAABB()
end

function ParticleLoadEnv:getMesh(meshDesc)
	local n = self:getNode(meshDesc)
	if n then
		local render = getRender(n)
		if render.getMesh then
			return render:getMesh()
		end
	else
		return g_meshManager:getMesh(meshDesc)
	end
end

function ParticleLoadEnv:deferMessage(delay, msg)
	if not self.messageReceiver then
		error('no messageReceiver')
	end
	
	g_scheduler:schedule(function ()
		if msg == 'destroy' then
			self.pobject:stop()
			self.pobject = nil
		end
		
		self.messageReceiver(msg)
	end, delay)
end
------------------------------------------------------------------------------

return ParticleLoadEnv