local functional = {}

function functional.bind1(f, _1)
	return function (...)
		return f(_1, ...)
	end
end

function functional.bind2(f, _1, _2)
	return function (...)
		return f(_1, _2, ...)
	end
end

if select('#', ...) == 0 then 
	local function plus(a, b) return a+b end
	
	local plus_two = functional.bind1(plus, 2)
	assert(plus_two(3) == 5)
	
	local one_plus_ten = functional.bind2(plus, 1, 10)
	assert(one_plus_ten() == 11)
end

return functional