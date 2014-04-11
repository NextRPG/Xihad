local class   = require "lbase.class"
local Variant = require "Variant"
local Tuner = class.def {}

function Tuner.new(filed, var)
	class.new(Tuner, {
		target = nil,
		filed = filed,
		var = var:copy(),
	})
	
	return obj
end

function Tuner:onUpdate(time)
	self.var:update(time)
	self:apply()
end

-- Abstract Functions
-- Tuner:lerp(from, to, ratio)
-- Tuner:setValue(target, value)
-- Tuner:stop()

if select('#', ...) == 0 then 
	local duration = 1.5 -- seconds
	local delta = math3d.vector(0, 0, 10)
	local t1 = Tuner.new("Translate", duration, delta)
	
	local from = math3d.vector()
	local to = math3d.vector(0, 90, 0)
	local t2 = Tuner.new("Rotate", duration, from, to)
	
	local t3 = Tuner.sequence(t1, 1, t2)	-- t1, delay 1 second, then t2
	local t4 = Tuner.parallel(t3, t2:copy())
	local t5 = Tuner.loop(t4, 3)
	function t5.endCallback()
		print("looped 3 times")
	end
	
	
end

return Tuner