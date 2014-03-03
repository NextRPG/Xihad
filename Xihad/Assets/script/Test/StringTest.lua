-- local a = {}
-- a[1] = {}
-- a[1][2] = 3
-- local str = "1.2"

-- local ref = a
-- for slice in str:gmatch("[^.].") do
-- 	ref = ref[tonumber(slice)]
-- end
-- local realKey = str:match("[^.]", -1)
-- ref[realKey] = 2
-- print(ref[realKey])
key = "properties.currentHP"
for slice in key:gmatch("([^\.]+)[\.]") do
	print(slice)
end