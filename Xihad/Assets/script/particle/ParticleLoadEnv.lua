local Env = { source = nil, target = nil, particleComponent = nil }

function Env.new(pcomp, source, target)
	local o = {
		particleComponent = pcomp,
		source = source,
		target = target	
	}
	setmetatable(o, Env)
	return o
end

function Env:getNode(objDesc)
	if nodeDesc:sub(1, 1) == '@' then
		local t = nodeDesc:sub(2)
		if t == 'source' or t == 'target' then
			return self[t]
		end
	end
end

function Env:getPosition(obj)
	return obj:getTranslation()
end

function Env:getAABB(obj)
	return obj:getAABB()
end

function Env:getMesh(meshDesc)
	local n = self:getNode(meshDesc)
	if n then
		if n.getMesh then
			return n:getMesh()
		end
	else
		return g_meshManager:getMesh(meshDesc)
	end
end

function Env:deferMessage(delay, msg)
	local msgParam = { source = self.source, target = self.target }
	local src = particleComponent:getHostObject():getID()
	g_scene:getDispatcher():dispatch(msg, msgParam, src, delay)
end

return Env