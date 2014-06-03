local base = require 'BaseParcel'
local Parcel = setmetatable({
		sMaxSlot = 5,
	}, base)
Parcel.__index = Parcel
Parcel.__base = 'BaseParcel'

function Parcel.new()
	return setmetatable(base.new(), Parcel)
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
	base.dropItem(item, delta)
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
