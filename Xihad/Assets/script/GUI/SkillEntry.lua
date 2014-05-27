local SkillEntry = {
	skillCaster = nil,
	skill = nil,
}
SkillEntry.__index = SkillEntry

function SkillEntry.new()
	local obj = setmetatable({ }, SkillEntry)
	return obj
end

function SkillEntry:onHover()
	
end

function SkillEntry:onSelected()
	
end

function SkillEntry:isEnabled()
	return skillCaster:canCast(skill)
end

function SkillEntry:getName()
	return skill:getName()
end

function SkillEntry:getValue()
	return skillCaster:getRestCount(skill)
end

return SkillEntry