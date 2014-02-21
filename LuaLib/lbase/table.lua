local t = {}

function t.copy(t, which)
	local which = which or "all"
	local cpy = {}
	
	if which == "array" then
		for _,v in ipairs(t) do
			table.insert(cpy, v)
		end
	end
	
	if which == "all" then
		for k,v in pairs(t) do
			cpy[k] = v
		end
	end
	
	return cpy
end

if select('#', ...) == 0 then 
	local proto = { 1, 2, a='a', b='b' }
	local cpy = t.copy(proto, "array")
	assert(cpy[1] == proto[1])
	assert(cpy[2] == proto[2])
	assert(cpy.a == nil)
	assert(cpy.b == nil)
	
	cpy = t.copy(proto, "all")
	assert(cpy[1] == proto[1])
	assert(cpy[2] == proto[2])
	assert(cpy.a == proto.a)
	assert(cpy.b == proto.b)
end

return t