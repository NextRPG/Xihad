local Array = require 'std.Array'
local Parcel = {
	slots = {},
	count = {},
	
	sMaxSlot = 5,
}
Parcel.__index = Parcel

function Parcel.new()
	return setmetatable({
			slots = {},
			count = {},
		}, Parcel)
end

function Parcel:_myWarrior()
	return self:findPeer(c'Warrior')
end

function Parcel:_set_slot(pos, item, cnt)
	self.slots[pos] = item
	self.count[pos] = cnt
end

function Parcel:_is_item_at(pos, item)
	return self.slots[pos] == item
end

function Parcel:_free_slot_count()
	return Parcel.sMaxSlot - #self.slots
end

function Parcel:_check_free_slot_count(n)
	if self:_free_slot_count() < n then
		self:_remove_dead_slot()
		error('no more slots')
	end
end

---
-- The calls to _new_slot() will might create a invalid slot.
-- So, if any exception throwing, we should remove it
function Parcel:_remove_dead_slot()
	if Array.getBack(self.count) == 0 then
		Array.popBack(self.count)
		Array.popBack(self.slots)
	end
end

function Parcel:_new_slot(item, cnt)
	self:_check_free_slot_count(1)
	
	local pos = #self.slots + 1
	self:_set_slot(pos, item, cnt or 0)
	return pos
end

function Parcel:_get_slot_for(item)
	return self:_last_pos(item) or self:_new_slot(item)
end

function Parcel:_clear_slot(pos)
	Array.fastRemoveElementAt(self.slots, pos)
	Array.fastRemoveElementAt(self.count, pos)
end

function Parcel:_check_can_insert_all(total, maxOverlay)
	self:_check_free_slot_count(math.ceil(total / maxOverlay))
end

function Parcel:_insert_extras(item, total)
	local maxOverlay = item:getMaxOverlay()
	assert(maxOverlay > 0 and total > 0)
	
	self:_check_can_insert_all(total, maxOverlay)
	
	local free = #self.slots + 1
	repeat
		self:_set_slot(free, item, math.min(total, maxOverlay))
		free = free + 1
		total = total - maxOverlay
	until total <= 0
end

function Parcel:_get_total(item, last)
	local total, occupiedSlotCount = 0, 0
	while last do
		assert(self:_is_item_at(last, item))
		
		occupiedSlotCount = occupiedSlotCount + 1
		total = self.count[last] + total
		last = self:_previous_pos(item, last)
	end
	
	return total, occupiedSlotCount
end

function Parcel:_previous_pos(item, pos)
	for i = pos-1, 1, -1 do
		if self:_is_item_at(i, item) then
			return i
		end
	end
end

function Parcel:_last_pos(item)
	return self:_previous_pos(item, #self.slots+1)
end

function Parcel:_checked_set(pos, count)
	if count <= 0 then
		self:_clear_slot(pos)
	else
		self.count[pos] = count
	end
end

function Parcel:_drop_recursive(item, pos, delta)
	if self:_get_total(item, pos) < delta then
		self:_remove_dead_slot()
		error('no enough item to drop')
	end
	
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

function Parcel:_inc_no_check(pos, delta)
	self.count[pos] = self.count[pos] + delta
end

function Parcel:dropItem(item, delta)
	assert(delta > 0)
	local pos = self:_get_slot_for(item)

	if self.count[pos] > delta then
		self:_inc_no_check(pos, -delta)
	else
		self:_drop_recursive(item, pos, delta)
	end
end

---
-- TODO rewrite this method to allow unevenness
function Parcel:gainItem(item, delta)
	assert(delta > 0)
	local pos = self:_get_slot_for(item)
	
	local maxGain = item:getMaxOverlay() - self.count[pos]
	if maxGain < delta then
		self:_insert_extras(item, delta - maxGain)
		delta = maxGain
	end
	
	self:_inc_no_check(pos, delta)
end

function Parcel:tidy()
	
end

function Parcel:useItem(itemIndex)
	
end

function Parcel:useItem(item)
	local usage, delta = item:onUsed(self:_myWarrior())
	self:dropItem(item, delta)
	return usage
end

function Parcel:canUse(item)
	return item:canUse(self:_myWarrior())
end

function Parcel:getTotal(item)
	return (self:_get_total(item, self:_last_pos(item)))
end

function Parcel:allItems()
	local idx = 0
	local function iter(self)
		idx = idx + 1
		return self.slots[idx], self.count[idx]
	end
	
	return iter, self
end

function Parcel:hasItem()
	return #self.slots > 0
end

if select('#', ...) == 0 then 
	local aItem = { getMaxOverlay = function() return 10 end }
	local bItem = { getMaxOverlay = function() return 1 end }
	local success
	local p = Parcel.new()
	
	p:gainItem(aItem, 10)
	assert(p:getTotal(aItem) == 10)
	assert(p:_free_slot_count() == 4)
	
	p:gainItem(aItem, 1)
	assert(p:getTotal(aItem) == 11)
	assert(p:_free_slot_count() == 3)
	
	p:dropItem(aItem, 2)
	assert(p:getTotal(aItem) == 9)
	assert(p:_free_slot_count() == 4)
	
	p:dropItem(aItem, 9)
	assert(p:getTotal(aItem) == 0)
	assert(p:_free_slot_count() == 5)
	
	p:gainItem(bItem, 4)
	assert(p:getTotal(bItem) == 4)
	assert(p:_free_slot_count() == 1)
	
	p:gainItem(aItem, 10)
	assert(p:getTotal(aItem) == 10)
	assert(p:_free_slot_count() == 0)
	
	p:dropItem(bItem, 3)
	assert(p:getTotal(bItem) == 1)
	assert(p:getTotal(aItem) == 10)
	assert(p:_free_slot_count() == 3)
	
	-- drop or gain exist item and error occured
	success = pcall(p.gainItem, p, bItem, 10) 
		and pcall(p.dropItem, p, aItem, 20)
	assert(not success)
	assert(p:getTotal(bItem) == 1)
	assert(p:getTotal(aItem) == 10)
	assert(p:_free_slot_count() == 3)
	
	-- drop or gain non-exist item and error occured
	p:dropItem(bItem, 1)
	success = pcall(p.gainItem, p, bItem, 10)
		and pcall(p.dropItem, p, bItem, 20)	
	assert(not success)
	assert(p:getTotal(bItem) == 0)
	assert(p:getTotal(aItem) == 10)
	assert(p:_free_slot_count() == 4)
end	

return Parcel
