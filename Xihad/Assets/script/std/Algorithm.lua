local Algo = {}

function Algo.all_of_t(t, f)
	for k,v in pairs(t) do
		if not f(k, v) then
			return false
		end
	end
	
	return true
end

function Algo.none_of_t(t, f)
	for k,v in pairs(t) do
		if f(k, v) then
			return false
		end
	end
	
	return true
end

function Algo.any_of_t(t, f)
	for k,v in pairs(t) do
		if f(k, v) then
			return true
		end
	end
	
	return false
end

function Algo.find_t(t, value)
	for k,v in pairs(t) do
		if v == value then
			return k
		end
	end
end

function Algo.find_if_t(t, f)
	for k,v in pairs(t) do
		if f(k, v) then
			return k
		end
	end
end

function Algo.count_t(t, value)
	local count = 0
	for k,v in pairs(t) do
		if v == value then
			count = count + 1
		end
	end
	
	return count
end

function Algo.count_if_t(t, f)
	local count = 0
	for k,v in pairs(t) do
		if f(k, v) then
			count = count + 1
		end
	end
	
	return count
end

function Algo.all_of(iter, f)
	for _1, _2, _3 in iter() do
		if not f(_1, _2, _3) then
			return false
		end
	end
	
	return true
end

function Algo.none_of(iter, f)
	for _1, _2, _3 in iter() do
		if f(_1, _2, _3) then
			return false
		end
	end
	
	return true
end

function Algo.any_of(iter, f)
	for _1, _2, _3 in iter() do
		if f(_1, _2, _3) then
			return true
		end
	end
	
	return false
end

function Algo.find_if(iter, f)
	for _1, _2, _3 in iter() do
		if f(_1, _2, _3) then
			return _1, _2, _3
		end
	end
end

function Algo.count_if(iter, f)
	local count = 0
	for _1, _2, _3 in iter() do
		if f(_1, _2, _3) then
			count = count + 1
		end
	end
	
	return count
end

function Algo.clamp(value, min, max)
	assert(min <= max, string.format('value: %d; min: %d; max: %d', value, min, max))
	return math.min(math.max(value, min), max)
end

function Algo.min_max(a, b)
	if a<b then
		return a, b 
	else
		return b, a
	end
end

function Algo.sign(n)
	if n > 0 then
		return 1
	elseif n < 0 then
		return -1
	else
		return 0
	end
end


return Algo
