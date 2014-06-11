local PromoteResult = require 'Warrior.PromoteResult'
local LevelGrader = {
	expToNextLevel= nil,	-- level -> exp
	skillsToLearn = nil,	-- level -> skills
	propertyIncrements = nil,
}
LevelGrader.__index = LevelGrader

function LevelGrader.new(expToNextLevel, skillsToLearn, propertyIncrements)
	assert(type(expToNextLevel)== 'function')
	assert(type(skillsToLearn) == 'table')
	assert(type(propertyIncrements) == 'table')

	return setmetatable({
			expToNextLevel= expToNextLevel,
			skillsToLearn = skillsToLearn,
			propertyIncrements = propertyIncrements,
		}, LevelGrader)
end

function LevelGrader:getNextLevelExp(currentLevel)
	assert(currentLevel >= 1)
	return self.expToNextLevel(currentLevel)
end

function LevelGrader:promote(warrior, atLevel)
	local result = PromoteResult.new()
	
	local skillList = self.skillsToLearn[atLevel]
	if skillList then
		result:setNewSkills(skillList)
	end
	
	for pname, _ in warrior:allProperties() do
		local pinc = self.propertyIncrements[pname]
		if pinc ~= nil then
			result:setPropertyInc(pname, pinc(warrior, atLevel))
		end
	end
	
	return result
end

return LevelGrader