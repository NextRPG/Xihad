--[[
update(deltaTime)
stopped()
copy()
reset()
--]]

local class = require "lbase.class"
local d = class.def {
	elapsed = 0,
}

function d.new(duration, easefunc)
	return class.new(d, {
		duration = duration,
		easefunc = easefunc
	})
end

function d:update(time)
	if not self:stopped() then
		self:onUpdate(time)
	end
end

function d:onUpdate(time)
	if self:stopped() then
		return
	end
	
	local progress
	self.elapsed = self.elapsed + time
	if self.elapsed >= self.duration then
		progress = 1
	else
		progress = self.elapsed / self.duration
	end
	
	if self.easefunc then
		progress = self.easefunc(progress)
	end
	
	self:onProgress(progress)
end

function d:copy()
	return class.new(d, {
		duration = self.duration,
		easefunc = self.easefunc
	})
end

function d:reset()
	self.elapsed = 0
end

function d:stopped()
	return self.elapsed >= self.duration
end

return d
