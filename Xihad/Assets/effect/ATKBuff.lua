require "math3d"
return function(pnode, f, env, scale)
	scale = scale or 1
	local buff = require "effect.buff"
	buff(pnode:newChild(), f, env, scale, Color.new(Color.red))
end