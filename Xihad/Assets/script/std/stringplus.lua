function string.split( str, spliter )
	local arr = {}
	for slice in str:gmatch("[^" .. spliter .. "]+") do
		arr[#arr + 1] = slice
	end
	return arr
end

function string.upperFirst( str )
	return str:sub(1, 1):upper() .. str:sub(2, #str)
end