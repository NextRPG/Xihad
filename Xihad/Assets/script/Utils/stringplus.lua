function string.split( str, spliter )
	local arr = {}
	for slice in str:gmatch("[^" .. spliter .. "]+") do
		arr[#arr + 1] = slice
	end
	return arr
end

local str = "haha.hehe"

-- print(string.split(str, "."))