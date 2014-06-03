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
	
end

function Parcel:_drop_redundants_to(view)
	local prevItem = nil
	for idx, item, count in self:allSlots() do 
		if prevItem ~= item then
			local totalCount = view:getTotalOf(item)
			local delta = totalCount - self:getTotalOf(item)
			if delta < 0 then
				self:dropItem(item, -delta)
			end
			
			prevItem = item
		end
	end
end

function Parcel:_gain_rest_of(view)
	local prevItem = nil
	for idx, item, count in view:allSlots() do 
		if prevItem ~= item then
			local totalCount = view:getTotalOf(item)
			local delta = totalCount - self:getTotalOf(item)
			assert(delta >= 0)
			if delta > 0 then
				self:gainItem(item, delta)
			end
			
			prevItem = item
		end
	end
end

function Parcel:restoreView(view)
	self:tidy()
	view:tidy()
	
	self:_drop_redundants_to(view)
	self:_gain_rest_of(view)
end

function Parcel:onDropItem(item, count)
	item:onDiscarded(self:findPeer(c'Warrior'), count)
end

function Parcel:dropItemAt(itemIndex, delta)
	local item = self:getItemAt(itemIndex)
	base.dropItemAt(self, itemIndex, delta)
	self:onDropItem(item, delta)
end

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
