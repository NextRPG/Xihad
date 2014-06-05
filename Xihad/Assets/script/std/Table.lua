local Table = {}

function Table.filteredPairs(t, filter)
	local function iter(t, lastKey) 
		local k, v
		
		repeat
			k, v = next(t, lastKey)
		until not k or filter(k, v)
			
		return k, v
	end
	
	return iter, t
end

function Table.keys(t)
	local f, s, v = pairs(t)
	local wrapped = function (s, v)
		return (f(s, v))
	end
	
	return wrapped, s, v
end

function Table.toSet(array)
	local set = {}
	for _, value in ipairs(array) do
		set[value] = true
	end
	
	return set
end

function Table.extractKeys(t)
	local keys = {}
	for k,v in pairs(t) do
		table.insert(keys, k)
	end
	
	return keys
end

function Table.clear(t)
	for k,v in pairs(t) do
		t[k] = nil
	end
end

function Table.get_or_set(t, key, default)
	local ret = t[key]
	if not ret then
		ret = default
		t[key] = default
	end
	
	return ret
end

function Table.get_or_new_table(t, key)
	local ret = t[key]
	if not ret then
		ret = {}
		t[key] = ret
	end
	
	return ret
end

function Table.count(t)
	local n = 0
	for k,v in pairs(t) do
		n = n + 1
	end
	
	return n
end

return Table