-- 暗光斩

return function(pnode, f, env, scale, color) 
	local blade = require "effect.light_blade"
	blade(pnode, f, env, scale, "rev_substract")
end