--- 
-- Action is a update handler which will finish at some time. And once 
-- it's finished, it can be stopped.
local d = {
	finishListeners = nil,
}
d.__index = d

function d.new()
	return setmetatable({}, d)
end

function d:update(time)
	if not self:hasFinished() then
		self:onUpdate(time)
		
		if self:hasFinished() and self.finishListeners then
			for listener, _ in pairs(self.finishListeners) do
				listener(self)
			end
		end
	end
end

function d:addFinishListener(lis)
	if not self.finishListeners then
		self.finishListeners = {}
	end
	
	self.finishListeners[lis] = true
end

function d:removeFinishListener(lis)
	if not self.finishListeners then
		return
	end
	
	self.finishListeners[lis] = nil
end

function d:reset()
	-- nothing to reset
end

function d:hasFinished()
	return false
end

return d
