-- function findMax( list )
-- 	local maxKey, maxValue = next(list)
-- 	for k,v in pairs(list) do
-- 		if maxValue < v then
-- 			maxKey, maxValue = k, v
-- 		end	
-- 	end
-- 	return maxKey, maxValue
-- end

-- function findMin( list )
-- 	local minKey, minValue = next(list)
-- 	for k,v in pairs(list) do
-- 		if minValue > v then
-- 			minKey, minValue = k, v
-- 		end
-- 	end
-- 	return minKey, minValue
-- end
-- list = { a = 1, b = 2}
-- print(findMin(list))
-- print(findMax(list))

function hex2Color( str )
	return tonumber("0x" .. str:sub(1, 2)), tonumber("0x" .. str:sub(3, 4)), tonumber("0x" .. str:sub(5, 6))
end

print(hex2Color("102b6a"))
