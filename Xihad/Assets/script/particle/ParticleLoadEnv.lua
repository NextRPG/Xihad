local Env = { source = nil, target = nil }

function Env:setSourceTarget(source, target)
	self.source = source
	self.target = target
end

function Env:getNode(objDesc)
	if nodeDesc:sub(1, 1) == '@' then
		local t = nodeDesc:sub(2)
		if t == 'source' or t == 'target' then
			return self[t]
		end
	end
end

function Env:setPosition(obj, pos)
	obj:resetTranslation(pos)
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
	end
	
	return meshManager:getMesh(meshDesc)
end

function Env:deferMessage(obj, delay, msg)
	local msgParam = { source = self.source, target = self.target }
	scene:getDispatcher():dispatch(msg, msgParam, obj:getID(), delay)
end

return Env