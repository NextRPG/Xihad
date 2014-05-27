local Equiper = { 
	equipments = nil,
}
Equiper.__index = Equiper

function Equiper.new()
	return setmetatable({
			equipments = {},
		}, Equiper)
end

function Equiper:getEquipment( etype )
	return self.equipments[etype]
end

function Equiper:equip( equipment )
	if not equipment:isEquiped(self) then
		local etype = equipment:getType()
		self:unequip(etype)
		assert(self.equipments[etype] == nil)
		
		equipment:promote(self:findPeer(c'Warrior'))
		self.equipments[etype] = equipment
	end
end

function Equiper:unequip( etype )
	local equipment = self.equipments[etype]
	
	if equipment then
		equipment:cancel(self:findPeer(c'Warrior'))
		self.equipments[etype] = nil
	end
end

return Equiper
