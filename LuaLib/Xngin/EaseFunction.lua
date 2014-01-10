local ease = {
	in2out = {},
	in2inout = {},
}
setmetatable(ease, ease)

function ease.linear(percent)
	return percent
end

function ease.quad(percent)
	return percent*percent
end

function ease.cubic(percent)
	return percent*percent*percent
end

function ease.quart(percent)
	return percent*percent*percent*percent
end

function ease.quint(percent)
	return percent*percent*percent*percent*percent
end

function ease.expo(percent)
	local rounding = 1/16777215
	local approx = math.pow(2, 24 * (percent-1))
	return approx + approx*rounding - rounding
end

function ease.sine(percent)
	return math.sin(percent * math.pi/2)
end

function ease._cos(percent)
	if percent == 1 then 
		return 1 -- fix rounding error
	else
		return 1 - math.cos(percent * math.pi/2)
	end
end

function ease.circ(percent)
	return 1 - math.sqrt(1 - percent * percent)
end

function ease.back(percent, overshoot)
	if percent == 1 then
		return 1 -- fix rounding error
	else
		overshoot = overshoot or 1.70158
		return percent * percent * ((overshoot + 1) * percent - overshoot)
	end
end

function ease.elastic(percent)
	local expo = ease.expo(percent)
	return - expo * math.sin((percent - 1.075)/0.3 * math.pi*2)
end

function ease.bounce(percent)
	local rp = (1 - percent) * 2.75
	
	if rp > 2.5 then
		local t = rp - 2.625
		return 0.015625 - t*t 	-- 0 -> 0.015625 -> 0
	elseif rp > 2 then
		local t = rp - 2.25
		return 0.0625 - t*t 	-- 0 -> 0.0625 -> 0
	elseif rp > 1 then
		local t = rp - 1.5
		return 0.25 - t*t 		-- 0 -> 0.25 -> 0
	else
		return 1 - rp*rp		-- 0 -> 1
	end
end

-- assert(out(1-x) + in(x) == 1)
function ease.wrapOut(infunc, notcache)
	local out = ease.in2out[infunc]
	if not out then
		local eout = require("EaseOutFunction")
		out = eout.new(infunc)
		assert(out.infunc == infunc)
		if not notcache then
			ease.in2out[infunc] = out
		end
	end
	
	return out
end

function ease.wrapInout(infunc, notcache)
	local inout = ease.in2inout[infunc]
	if not inout then
		local einout = require("EaseInOutFunction")
		inout = einout.new(infunc, ease.wrapOut(infunc, notcache))
		assert(inout.infunc == infunc)
		if not notcache then
			ease.in2inout[infunc] = inout
		end
	end
	return inout
end
ease.wrapInOut = ease.wrapInout

function ease.extractEaseIn(wrapped)
	if wrapped then
		return wrapped.infunc
	end
end

function ease:__call(func, inout, percent, ex)
	local targetfunc
	local infunc = self[func]
	
	if not infunc or not inout or inout == 'in' then
		targetfunc = infunc
	elseif inout == 'out' or inout == 'inout' then
		if inout == 'inout' then
			targetfunc = ease.wrapInout(infunc)
		else
			targetfunc = ease.wrapOut(infunc)
		end
	end
	
	if targetfunc and percent then
		return targetfunc(percent, ex)
	else
		return targetfunc
	end
end

ease[1] = ease.linear
ease[2] = ease.quad
ease[3] = ease.cubic
ease[4] = ease.quart
ease[5] = ease.quint
if select('#', ...) == 0 then
	-- 直接计算
	assert(ease("cubic", "in" , 0.5) ==   0.125)
	assert(ease("cubic", "out", 0.5) == 1-0.125)
	assert(ease("cubic", "inout", 0.5) == 0.500)
	
	-- 获取函数
	local easeQuadIn = ease("quad")
	assert(easeQuadIn == ease("quad", "in"))
	assert(easeQuadIn == ease(2, "in"))
	assert(easeQuadIn == ease.quad)
	assert(easeQuadIn == ease[2])
	assert(easeQuadIn(0.5) == 0.25)
	
	-- 包装函数
	local easeQuintOut = ease.wrapOut(ease.quint)
	assert(easeQuintOut == ease.wrapOut(ease.quint))
	assert(easeQuintOut == ease("quint", "out"))
	
	local easeQuintInOut = ease.wrapInout(ease.quint)
	assert(easeQuintInOut == ease.wrapInout(ease.quint))
	assert(easeQuintInOut == ease("quint", "inout"))
	
	assert(ease.extractEaseIn(easeQuintOut) == ease.quint)
	assert(ease.extractEaseIn(easeQuintInOut) == ease.quint)
	
	-- 数值测试
	assert(ease.expo(0)    == 0 and ease.expo(1) 	== 1)
	assert(ease.sine(0)    == 0 and ease.sine(1) 	== 1)
	assert(ease._cos(0)    == 0 and ease._cos(1) 	== 1)
	assert(ease.circ(0)    == 0 and ease.circ(1) 	== 1)
	assert(ease.back(0)    == 0 and ease.back(1) 	== 1)
	assert(ease.bounce(0)  == 0 and ease.bounce(1) 	== 1)
	assert(ease.elastic(0) == 0 and ease.elastic(1) == 1)
end

return ease