Equipment = {
	part = "",
	category = "",
	name = "",
}
package.loaded["Equipment"] = Equipment

require "Consts"

function Equipment:new( o )
	o = o or {}
	-- o.effect = o.effect or {}
	o.buffs = o.buffs or {}

	setmetatable(o, {__index = self})
	return o
end

function Equipment:equip( character )
	-- not allow two same parts
	if character.equipments[part] ~= nil then
		equipment:unequip( character )
	end

	for k,equation in pairs(character.properties) do
		equation:addPower(self.buffs[k].power)
		equation:addFixed(self.buffs[k].fixed)
	end
end

function Equipment:unequip( character )
	for k,equation in pairs(character.properties) do
		equation:reducePower(self.buffs[k].power)
		equation:reduceFixed(self.buffs[k].fixed)
	end
end

