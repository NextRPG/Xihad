local base = require 'BaseParcel'
local Array = require 'std.Array'
local Parcel = setmetatable({
		sMaxSlot = 5,
	}, base)
Parcel.__index = Parcel
Parcel.__base = 'BaseParcel'

function Parcel.new()
	return setmetatable(base.new(), Parcel)
end

function Parcel:createView()
	local view = base.new()
	function view.getTotalSlotCount(view) 
		return self:getTotalSlotCount()
	end
	
	function view:getValue(index)
		return string.format('%2d', self:getCountAt(index))
	end
	
	view.slots = Array.copy(self.slots)
	view.count = Array.copy(self.count)
	return view
end

local function allItemsOfTidiedParcel(parcel)
	local function iter(parcel, idx)
		local prevItem
		if not idx then
			idx = 0
		else
			prevItem = parcel:getItemAt(idx)
		end
		
		local item = nil
		repeat
			idx = idx + 1
			item = parcel:getItemAt(idx)
			
			if not item then
				return nil
			end
		until item ~= prevItem
		
		return idx, item, parcel:getTotalOf(item)
	end
	
	return iter, parcel, nil
end

function Parcel:_apply_change_list(method, changeList)
	for item, delta in pairs(changeList) do
		self[method](self, item, delta)
	end
end

function Parcel:_drop_redundants_to(view)
	local changeList = {}
	for _, item, count in allItemsOfTidiedParcel(self) do
		local totalCount = view:getTotalOf(item)
		if totalCount < count then
			changeList[item] = count - totalCount
		end
	end
	
	self:_apply_change_list('dropItem', changeList)
end

function Parcel:_gain_rest_of(view)
	local changeList = {}
	for _, item, totalCount in allItemsOfTidiedParcel(view) do
		local myCount = self:getTotalOf(item)
		if totalCount > myCount then
			changeList[item] = totalCount - myCount
		end
	end
	
	self:_apply_change_list('gainItem', changeList)
end

function Parcel:restoreView(view)
	self:tidy()
	view:tidy()
	
	self:_drop_redundants_to(view)
	self:_gain_rest_of(view)
	
	for _, item, count in allItemsOfTidiedParcel(view) do
		assert(self:getTotalOf(item) == count)
	end
	
	for _, item, count in allItemsOfTidiedParcel(self) do
		assert(view:getTotalOf(item) == count)
	end
end

function Parcel:onDropItem(item, count)
	item:onDiscarded(self:findPeer(c'Warrior'), count)
end

function Parcel:dropItemAt(itemIndex, delta)
	local item = self:getItemAt(itemIndex)
	base.dropItemAt(self, itemIndex, delta)
	self:onDropItem(item, delta)
end

-- function Parcel:gainItem(item, delta)
-- 	local success = pcall(base.gainItem, self, item, delta)
-- 	-- TODO
-- end

function Parcel:dropItem(item, delta)
	base.dropItem(self, item, delta)
	self:onDropItem(item, delta)
end

function Parcel:getTotalSlotCount()
	return Parcel.sMaxSlot
end

function Parcel:useItemAt(itemIndex)
	local item = self:_check_item_at(itemIndex)
	local usage, loss = item:onUsed(self:findPeer(c'Warrior'))
	if loss then
		self:dropItemAt(itemIndex, 1)
	end
	
	return usage
end

function Parcel:getValue(itemIndex)
	local item = self:getItemAt(itemIndex)
	assert(item ~= nil)
	
	local value
	if type(item.isEquiped) == 'function' then
		value = item:isEquiped(self:findPeer(c'Equiper')) and ' E' or ' N'
	else
		value = string.format('%2d', self:getCountAt(itemIndex))
	end
	
	return value
end

---
-- @Deprecated invoke useItemAt() instead
function Parcel:useItem(item)
	local pos = self:_last_pos(item)
	if not pos then
		error('no such a item')
	end
	
	return self:useItemAt(pos)
end

return Parcel
