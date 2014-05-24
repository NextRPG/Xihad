local base = require 'Item.BaseItem'
local EquipmentItem = setmetatable({
		_maxOverlay = 1,
		_occupyRound= false,
		etype = nil,
		
		effects = nil,
	}, BaseItem)
EquipmentItem.__index = EquipmentItem

function EquipmentItem.new(name, icon, desc, etype)
	local o = setmetatable(base.new(name, icon, desc), EquipmentItem)
	o.etype = etype
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
	for _, effect in ipairs(self.effects) do
		effect:copy():bindSticky(warrior, self)
	end
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

function EquipmentItem:onUsed(warrior)
	local equiper = equiperOf(warrior)
	
	if equiper then
		if self:isEquiped(equiper) then
			equiper:unequip(self.etype)
		else
			equiper:equip(self)
		end
	end
	
	return 0
end

return EquipmentItem
