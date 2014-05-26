local WarriorSerializer = {}
WarriorSerializer.__index = WarriorSerializer

function WarriorSerializer.new()
	return setmetatable({}, WarriorSerializer)
end

function WarriorSerializer:createSerializable()
	local warrior= self:findPeer(c'Warrior')
	local caster = self:findPeer(c'SkillCaster')
	local parcel = self:findPeer(c'Parcel')
	local leveler= self:findPeer(c'Leveler')
	
	local d = {
		-- model = ???,
		properties = {},
		career = warrior:getCareer(),
		skills = {},
		items  = {},
		level  = leveler:getLevel(),
		currentEXP = leveler:getCurrentExp(),
	}
	
	for property, value in warrior:allProperties() do
		d.properties[property] = value
	end
	
	for skill, _, total in caster:allSkills() do
		d.skills[skill:getName()] = total
	end
	
	parcel:tidy()
	for item, count in parcel:allSlots() do
		items[item:getName()] = count
	end
	
	return d
end

return WarriorSerializer
