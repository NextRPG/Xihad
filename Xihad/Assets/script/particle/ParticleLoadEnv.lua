local ParticleLoaderEnv = { 
	source = nil, 
	target = nil, 
	messageReceiver = nil,
	particleComponent = nil,
}
ParticleLoaderEnv.__index = ParticleLoaderEnv

local function getRender(obj)
	return obj:findComponent(c'Render')
end

function ParticleLoaderEnv.new(pcomp, source, target, msgReceiver)
	return setmetatable({
		particleComponent = pcomp,
		source = source,
		target = target,
		messageReceiver = msgReceiver,
	}, ParticleLoaderEnv)
end

function ParticleLoaderEnv:getNode(nodeDesc)
	if nodeDesc:sub(1, 1) == '@' then
		local t = nodeDesc:sub(2)
		if t == 'source' or t == 'target' then
			return self[t]
		end
	end
end

function ParticleLoaderEnv:getPosition(obj)
	return obj:getTranslate()
end

function ParticleLoaderEnv:getAABB(obj)
	return getRender(obj):getAABB()
end

function ParticleLoaderEnv:getMesh(meshDesc)
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

function ParticleLoaderEnv:deferMessage(delay, msg)
	g_scheduler:schedule(function ()
		self.messageReceiver(msg)
	end, delay)
end

return ParticleLoaderEnv