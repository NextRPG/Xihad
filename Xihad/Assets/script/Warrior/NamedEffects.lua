local NamedEffects = {
	datas = nil,
}
NamedEffects.__index = NamedEffects

function NamedEffects.new()
	return setmetatable({
			datas = {},
		}, NamedEffects)
end

function NamedEffects:attach(type, effect)
	local detached = self:_detach_no_check(type)
	
	self.datas[type] = effect
	return detached
end

function NamedEffects:_detach_no_check(type)
	local attached = self.datas[type]
	
	if attached then
		self.datas[type] = nil
	
		attached:unbind()
		
		return attached
	end
end

function NamedEffects:detach(type, effect)
	assert(effect)
	local attached = self.datas[type]
	
	if attached == effect then
		return self:_detach_no_check(type)
	elseif attached then
		print('non-matched detach', attached, effect)
	end
end


return NamedEffects