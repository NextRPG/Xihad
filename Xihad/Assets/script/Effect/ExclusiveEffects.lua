local ExclusiveEffects = {
	datas = nil,
}
ExclusiveEffects.__index = ExclusiveEffects

function ExclusiveEffects.new()
	return setmetatable({
			datas = {},
		}, ExclusiveEffects)
end

function ExclusiveEffects:attach(effect, exclusiveLock)
	local detached = self:_detach_initiatively(exclusiveLock)
	
	self.datas[exclusiveLock] = effect
	return detached
end

function ExclusiveEffects:_detach_initiatively(exclusiveLock)
	local attached = self.datas[exclusiveLock]
	
	if attached then
		self.datas[exclusiveLock] = nil
	
		attached:unbind()
		
		return attached
	end
end

function ExclusiveEffects:_detach_passively(exclusiveLock, effect)
	assert(effect)
	local attached = self.datas[exclusiveLock]
	
	if attached == effect then
		self.datas[exclusiveLock] = nil
		return attached
	elseif attached then
		print('non-matched detach', attached, effect)
	end
end

function ExclusiveEffects:detach(effect, exclusiveLock)
	return self:_detach_passively(exclusiveLock, effect)
end


return ExclusiveEffects