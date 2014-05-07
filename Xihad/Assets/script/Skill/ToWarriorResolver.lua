local ToWarriorResolver = {}
ToWarriorResolver.__index = ToWarriorResolver

function ToWarriorResolver.new()
	return setmetatable({}, ToWarriorResolver)
end

function ToWarriorResolver:resolve(sourceWarrior, barrier, relativeLoc, result)
	if not barrier.findPeer then return end
	
	local targetWarrior = barrier:findPeer(c'Warrior')
	
	if not targetWarrior then return end
	
	self:_resolve(sourceWarrior, targetWarrior, relativeLoc, result)
end

return ToWarriorResolver