local functional = require 'std.functional'
local Iterator = {}

function Iterator.filter2(passFunc, f, s, v)
	local function newIter(s, var) 
		local var1, var2
		
		repeat
			var1, var2 = f(s, var)
		until not var1 or passFunc(var1, var2)
			
		return var1, var2
	end
	
	return newIter, s, v
end

function Iterator.transform(func, f, s, v)
	return functional.cascade(func, f), s, v
end

return Iterator
