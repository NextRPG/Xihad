local SkillTargetFilter = {
	toEnemy	= true,
	toLeague= false,
	toSelf  = false,
	toVacancy = false,
}
SkillTargetFilter.__index = SkillTargetFilter

function SkillTargetFilter.new()
	local obj = setmetatable({ }, SkillTargetFilter)
	return obj
end

return SkillTargetFilter
