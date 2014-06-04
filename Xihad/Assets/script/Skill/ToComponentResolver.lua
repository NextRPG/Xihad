local ToComponentResolver = {
	componentName = nil,
}
ToComponentResolver.__index = ToComponentResolver

function ToComponentResolver.new(componentName)
	assert(type(componentName) == 'string')
	return setmetatable({
			componentName = c(componentName),
		}, ToComponentResolver)
end

function ToComponentResolver:resolve(source, barrier, relativeLoc, result)
	if not barrier.findPeer then return end
	
	local targetWarrior = barrier:findPeer(self.componentName)
	
	if not targetWarrior then return end
	
	self:_resolve(source, targetWarrior, relativeLoc, result)
end

return ToComponentResolver