local class = require "lbase.class"
local base 	= require "TimedUpdater"
local Variant = class.def({}, base)

function Variant.new(duration, from, stop, easefunc, lerpfunc)
	local o = base.new(duration, easefunc)
	o.lerpfunc = lerpfunc or function (f, t, r)
		return f + (t-f) * r
	end
	
	o.from = from
	o.stop = stop
	o.current = o.from
	return class.new(Variant, o)
end

function Variant:onProgress(progress)
	self.current = self.lerpfunc(self.from, self.stop, progress)
end

function Variant:copy()
	local cpy = base.copy(self)
	
	cpy.lerpfunc = self.lerpfunc
	cpy.from = self.from
	cpy.stop = self.stop
	cpy.current = cpy.from
	return class.new(Variant, cpy)
end

function Variant:reset()
	base.reset(self)
	self.current = self.from
end

class.accessor(Variant, "current", "readonly")

if select('#', ...) == 0 then 
	local ease = require "EaseFunction"
	local v = Variant.new(5, 10, 20, ease.back)
	
	local dt = 0.5
	local tm = 0
	while tm<3 do
		tm = tm + dt
		v:update(dt)
		print(v:getCurrent())
	end
	assert(not v:stopped())
	
	while tm<10 do
		tm = tm + dt
		v:update(dt)
		print(v:getCurrent())
	end
	assert(v:stopped())
	
	local vcpy = v:copy()
	assert(vcpy.from == v.from)
	assert(vcpy.stop == v.stop)
	assert(vcpy.current == vcpy.from)
	assert(vcpy.easefunc == v.easefunc)
	assert(vcpy.lerpfunc == v.lerpfunc)
end

return Variant
