local Table = {}

function Table.keys(t)
	local f, s, v = pairs(self.skills)
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