local Ease 	= require 'Ease.EaseFunction'
local Action= require 'Action.Action'

local d = {
	elapsed  = 0,
	easefunc = Ease.linear,
}
d.__index = d
setmetatable(d, Action)

function d.new(duration, easefunc)
	assert(duration >= 0)
	
	local o = Action.new()
	o.duration = duration
	o.easefunc = easefunc
	
	setmetatable(o, d)
	return o
end

function d:onProgress(progress)
	error("Derived class must implement this function")
end

function d:onUpdate(time)
	local progress
	self.elapsed = self.elapsed + time
	if self.elapsed >= self.duration then
		progress = 1
	else
		progress = self.elapsed / self.duration
	end
	
	progress = self.easefunc(progress)
	self:onProgress(progress)
end

function d:reset()
	self.elapsed = 0
end

function d:hasFinished()
	return self.elapsed >= self.duration
end

return d
