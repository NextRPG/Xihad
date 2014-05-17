-- 暗光斩

return function(pnode, f, env, color) 
	local blade = require "effect.light_blade"
	blade(pnode, f, env, "rev_substract")
end