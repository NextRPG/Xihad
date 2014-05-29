local Array = require 'std.Array'
local BaseParcel = {
	slots = {},
	count = {},
}
BaseParcel.__index = BaseParcel

function BaseParcel.new()
	return setmetatable({
			slots = {},
			count = {},
		}, BaseParcel)
end

------------------------------------------------------------------------------
-- Slot operations begin
function BaseParcel:_new_slot(item)
	if self:getFreeSlotCount() < 1 then
		error('no more slots')
	end
	
	local pos = #self.slots + 1
	self.slots[pos], self.count[pos] = item, 0
	
	return pos
end

function BaseParcel:_swap_slot(from, to)
	self.slots[from], self.slots[to] = self.slots[to], self.slots[from]
	self.count[from], self.count[to] = self.count[to], self.count[from]
end

function BaseParcel:_clear_slot(pos)
	Array.fastRemoveElementAt(self.slots, pos)
	Array.fastRemoveElementAt(self.count, pos)
end
-- Slot operations end
------------------------------------------------------------------------------


function BaseParcel:_is_item_at(pos, item)
	return self.slots[pos] == item
end

function BaseParcel:_get_total(item, last)
	local total, occupiedSlotCount = 0, 0
	while last do
		assert(self:_is_item_at(last, item))
		
		occupiedSlotCount = occupiedSlotCount + 1
		total = self.count[last] + total
		last = self:_previous_pos(item, last)
	end
	
	return total, occupiedSlotCount
end


------------------------------------------------------------------------------
-- Position iterators begin
function BaseParcel:_last_pos(item)
	return self:_previous_pos(item, #self.slots+1)
end

function BaseParcel:_previous_pos(item, pos)
	for i = pos - 1, 1, -1 do
		if self:_is_item_at(i, item) then
			return i
		end
	end
end

function BaseParcel:_first_pos(item)
	return self:_next_pos(item, 0)
end

function BaseParcel:_next_pos(item, pos)
	for i = pos+1, #self.slots do
		if self:_is_item_at(i, item) then
			return i
		end
	end
end

function BaseParcel:_checked_set(pos, count)
	if count <= 0 then
		self:_clear_slot(pos)
	else
		self.count[pos] = count
	end
end
-- Position iterators end
------------------------------------------------------------------------------


---
-- We must do it in reverse order because of the implementation 
-- of _clear_slot()
function BaseParcel:_drop_recursive(item, delta)
	local pos = self:_last_pos(item)
	while true do
		delta = delta - self.count[pos]
		if delta <= 0 then
			self:_checked_set(pos, -delta)
			return
		end
		
		self:_clear_slot(pos)
		pos = self:_previous_pos(item, pos)
		assert(pos)
	end
end

function BaseParcel:dropItemAt(itemIndex, delta)
	assert(delta > 0)
	local newCount = self.count[itemIndex] - delta
	if newCount < 0 then
		error('no enough item to drop')
	end
	
	self:_checked_set(itemIndex, newCount)
end

function BaseParcel:dropItem(item, delta)
	assert(delta > 0)
	if self:getTotalOf(item) < delta then
		error('no enough item to drop')
	end
	
	self:_drop_recursive(item, delta)
end

function BaseParcel:_check_can_insert_all(item, total)
	local maxOverlay = item:getMaxOverlay()
	local freeContain= self:getFreeSlotCount() * maxOverlay
	local mergeCount = total - freeContain
	if mergeCount > 0 then
		-- We must check exist ones
		local existing, usedSlots = self:_get_total(item)
		if existing + mergeCount > usedSlots*maxOverlay then
			error('no more slots')
		end
	end
end

function BaseParcel:_inc_no_check(pos, delta)
	self.count[pos] = self.count[pos] + delta
end

function BaseParcel:_fill(pos, delta, maxOverlay)
	local free = maxOverlay - self.count[pos]
	self:_inc_no_check(pos, math.min(free, delta))
	
	return delta - free
end

function BaseParcel:gainItem(item, delta)
	assert(delta > 0)
	self:_check_can_insert_all(item, delta)
	local pos = self:_first_pos(item)
	local maxOverlay = item:getMaxOverlay()
	while delta > 0 do
		pos = pos or self:_new_slot(item)
		delta = self:_fill(pos, delta, maxOverlay)
		pos = self:_next_pos(item, pos)
	end
end

function BaseParcel:_check_item_at(itemIndex)
	local item = self.slots[itemIndex]
	if not item then
		error('Invalid index')
	end
	
	return item
end

function BaseParcel:_tidy_at(startPos)
	local item = self.slots[startPos]
	local maxOverlay = item:getMaxOverlay()
	
	local nextPos = self:_next_pos(item, startPos)
	while nextPos do
		local rest = self:_fill(startPos, self.count[nextPos], maxOverlay)
		if rest > 0 then
			self.count[nextPos] = rest
			self:_swap_slot(nextPos, startPos + 1)
			startPos = startPos + 1
		else
			self:_clear_slot(nextPos)
		end
		
		nextPos = self:_next_pos(item, nextPos)
	end
	
	return startPos + 1	-- tidy ok
end

function BaseParcel:tidy()
	local pos = 1
	while pos < #self.slots do
		pos = self:_tidy_at(pos)
	end
end

function BaseParcel:getTotalOf(item)
	return (self:_get_total(item, self:_last_pos(item)))
end

function BaseParcel:getUsedSlotCount()
	return #self.slots
end

function BaseParcel:getTotalSlotCount()
	error('no implementation by default')
end

function BaseParcel:getFreeSlotCount()
	return self:getTotalSlotCount() - #self.slots
end

function BaseParcel:getItemAt(itemIndex)
	return self.slots[itemIndex]
end

function BaseParcel:getCountAt(itemIndex)
	return self.count[itemIndex]
end

---
-- for index, item, count in parcel:allSlots() do
-- 	...
-- end
function BaseParcel:allSlots()
	local function iter(self, idx)
		idx = idx + 1
		local item = self.slots[idx]
		if not item then idx = nil end
		return idx, item, self.count[idx]
	end
	
	return iter, self, 0
end

function BaseParcel:hasItem(item)
	if item then
		return self:_first_pos() ~= nil
	else
		return #self.slots > 0
	end
end

if select('#', ...) == 0 then 
	local aItem = { getMaxOverlay = function() return 10 end }
	local bItem = { getMaxOverlay = function() return 1 end }
	local success
	local p = BaseParcel.new()
	function p:getTotalSlotCount() return 5 end
	
	p:gainItem(aItem, 10)
	assert(p:getTotalOf(aItem) == 10)
	assert(p:getFreeSlotCount() == 4)
	
	p:gainItem(aItem, 1)
	assert(p:getTotalOf(aItem) == 11)
	assert(p:getFreeSlotCount() == 3)
	
	p:dropItem(aItem, 2)
	assert(p:getTotalOf(aItem) == 9)
	assert(p:getFreeSlotCount() == 4)
	
	p:dropItem(aItem, 9)
	assert(p:getTotalOf(aItem) == 0)
	assert(p:getFreeSlotCount() == 5)
	
	p:gainItem(bItem, 4)
	assert(p:getTotalOf(bItem) == 4)
	assert(p:getFreeSlotCount() == 1)
	
	p:gainItem(aItem, 10)
	assert(p:getTotalOf(aItem) == 10)
	assert(p:getFreeSlotCount() == 0)
	
	p:dropItem(bItem, 3)
	assert(p:getTotalOf(bItem) == 1)
	assert(p:getTotalOf(aItem) == 10)
	assert(p:getFreeSlotCount() == 3)
	
	-- drop or gain exist item and error occured
	success = pcall(p.gainItem, p, bItem, 10) 
		and pcall(p.dropItem, p, aItem, 20)
	assert(not success)
	assert(p:getTotalOf(bItem) == 1)
	assert(p:getTotalOf(aItem) == 10)
	assert(p:getFreeSlotCount() == 3)
	
	-- drop or gain non-exist item and error occured
	p:dropItem(bItem, 1)
	success = pcall(p.gainItem, p, bItem, 10)
		and pcall(p.dropItem, p, bItem, 20)	
	assert(not success)
	assert(p:getTotalOf(bItem) == 0)
	assert(p:getTotalOf(aItem) == 10)
	assert(p:getFreeSlotCount() == 4)
	
	p:gainItem(aItem, 30)
	assert(p:getFreeSlotCount() == 1)
	p:dropItemAt(1, 5)
	p:dropItemAt(3, 2)
	p:dropItemAt(4, 9)
	assert(p:getTotalOf(aItem) == 24)
	assert(p:getFreeSlotCount() == 1)
	
	p:tidy()
	assert(p:getTotalOf(aItem) == 24)
	assert(p:getFreeSlotCount() == 2)
end	

return BaseParcel