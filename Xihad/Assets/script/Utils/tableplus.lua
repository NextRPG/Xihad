function table.copy( t )
	local t_copy = {}
	for k,v in pairs(t) do
		if type(v) ~= "table" then
			t_copy[k] = v
		else
			t_copy[k] = table.copy(v)
		end
	end
	return t_copy
end

function table.equal( t1, t2 )
	for k,v in pairs(t1) do
		if type(v) == "table" and type(t2[k]) =="table" then
			if table.equal(v, t2[k]) == false then
				return false
			end
		elseif v ~= t2[k] and v ~= nil and t2[k] ~= nil then 
			return false 
		end
	end
	return true
end

function table.reverse( t )
	local i = 1
	while i < (#t + 1 - i) do
		t[i],t[#t + 1 - i] = t[#t + 1 - i],t[i]
		i = i + 1
	end
end

function table.contains( t, e )
	for k,v in pairs(t) do
		if v == e then return true end
		if table.equal(v, e) then return true end
	end
	return false
end