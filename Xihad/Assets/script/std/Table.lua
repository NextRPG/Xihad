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

return Table