local Array= require 'std.Array'
local OrderedSet = {
	data = nil,
}
OrderedSet.__index = OrderedSet

function OrderedSet.new()
	return setmetatable({
			data = {}
		}, OrderedSet)
end

function OrderedSet:size()
	return #self.data
end

function OrderedSet:insert(value)
	local idx = Array.bsearch(self.data, value)
	if idx < 0 then
		Array.insert(self.data, -idx, value)
	end
end

function OrderedSet:values()
	return ipairs(self.data)
end

function OrderedSet:contains(value)
	return Array.bsearch(self.data, value) > 0
end

return OrderedSet