local Algo = {}

function Algo.all_of(t, f)
	for k,v in pairs(t) do
		if not f(k, v) then
			return false
		end
	end
	
	return true
end

function Algo.none_of(t, f)
	for k,v in pairs(t) do
		if f(k, v) then
			return false
		end
	end
	
	return true
end

function Algo.any_of(t, f)
	for k,v in pairs(t) do
		if f(k, v) then
			return true
		end
	end
	
	return false
end

function Algo.find(t, value)
	for k,v in pairs(t) do
		if v == value then
			return k
		end
	end
end

function Algo.find_if(t, f)
	for k,v in pairs(t) do
		if f(k, v) then
			return k
		end
	end
end

function Algo.count(t, value)
	local count = 0
	for k,v in pairs(t) do
		if v == value then
			count = count + 1
		end
	end
	
	return count
end

function Algo.count_if(t, f)
	local count = 0
	for k,v in pairs(t) do
		if f(k, v) then
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


return Algo
