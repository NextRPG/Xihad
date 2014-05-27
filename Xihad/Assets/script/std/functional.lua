local functional = {}

function functional.notNil(p)
	return p ~= nil
end

function functional.passby(f, ...)
	f()
	return ...
end

function functional.return_(...)
	return ...
end

function functional.invoke(target, fnameOrParam, ...)
	if type(target) == "function" then
		local param = fnameOrParam
		return target(param, ...)
	elseif type(target) == "table" then
		local fname = fnameOrParam
		return target[fname](target, ...)
	end
end

function functional.cascade(f1, f2, ...)
	return f1(f2(...))
end

function functional.cascadeN(fs, ...)
	local n = #fs
	local top = fs[n]
	if n > 1 then
		local nxt = fs[n-1]
		table.remove(fs)
		return nxt(top(...))
	else
		return top(...)
	end
end

function functional.bindself(t, fname)
	local f = t[fname]
	return functional.bind1(f, t)
end

function functional.bind1(f, _1)
	assert(f)
	return function (...)
		return f(_1, ...)
	end
end

function functional.bind2(f, _1, _2)
	assert(f)
	return function (...)
		return f(_1, _2, ...)
	end
end

function functional.bind3(f, _1, _2, _3)
	assert(f)
	return function (...)
		return f(_1, _2, _3, ...)
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