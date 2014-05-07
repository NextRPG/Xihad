local Env = { 
	source = nil, 
	target = nil, 
	particleComponent = nil,
}
Env.__index = Env

local function getRender(obj)
	return obj:findComponent(c'Render')
end

function Env.new(pcomp, source, target)
	return setmetatable({
		particleComponent = pcomp,
		source = source,
		target = target	
	}, Env)
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
	return getRender(obj):getAABB()
end

function Env:getMesh(meshDesc)
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

function Env:deferMessage(delay, msg)
	local msgParam = { source = self.source, target = self.target }
	local src = particleComponent:getHostObject():getID()
	g_scene:getDispatcher():dispatch(msg, msgParam, src, delay)
end

return Env