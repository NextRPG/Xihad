--- 
-- Set类用来存储在table中只能出现一次的数据
-- @module Set
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Set = {
	_size = 0,
	_data = nil,
}
Set.__index = Set

function Set.new()
	o = {}
	setmetatable(o, Set)
	
	o._data = {}
	
	return o
end

function Set:contains(e)
	return self._data[e] ~= nil
end

function Set:insert(e)
	if not self._data[e] then
		self._data[e] = true
		self._size = self._size + 1
	end
end

function Set:remove(e)
	if self._data[e] then
		self._data[e] = nil
		self._size = self._size - 1
	end
end

function Set:clear()
	self._size = 0
	for k,v in pairs(self._data) do
		self._data[k] = nil
	end
end

function Set:size()
	return self._size
end

function Set:empty()
	return self._size ~= 0
end

return Set