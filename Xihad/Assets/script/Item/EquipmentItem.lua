local base = require 'Item.BaseItem'
local EquipmentItem = setmetatable({
		_maxOverlay = 1,
		_occupyRound= false,
		etype = nil,
		effect= nil,
	}, base)
EquipmentItem.__index = EquipmentItem

local equipUsageMT = { 
	__index = {
		equip 	= '@warrior装备了@item',
		unequip = '@warrior卸下了@item',
	}
}

function EquipmentItem.new(name, icon, desc, etype, effect)
	local o = setmetatable(base.new(name, icon, desc), EquipmentItem)
	o.etype = etype
	o.effect= effect
	setmetatable(o.usages, equipUsageMT)
	print(o.usages.equip)
	
	return o
end

function EquipmentItem:getType()
	return self.etype
end

local function equiperOf(warrior)
	return warrior:findPeer(c'Equiper')
end

function EquipmentItem:isEquiped(equiper)
	return equiper:getEquipment(self.etype) == self
end

function EquipmentItem:promote(warrior)
	self.effect:copy():bindSticky(warrior, self)
end

function EquipmentItem:cancel(warrior)
	warrior:unbindStickyEffects(self)
end

function EquipmentItem:onDiscarded(warrior, count)
	local equiper = equiperOf(warrior)
	
	if equiper then
		if self:isEquiped(equiper) then
			equiper:unequip(self.etype)
		end
	end
end

function EquipmentItem:_onUsed(warrior)
	local equiper = equiperOf(warrior)
	assert(equiper ~= nil)
	
	local usage 
	if self:isEquiped(equiper) then
		equiper:unequip(self.etype)
		usage = 'unequip'
	else
		equiper:equip(self)
		usage = 'equip'
	end
	
	return usage, false
end

return EquipmentItem
