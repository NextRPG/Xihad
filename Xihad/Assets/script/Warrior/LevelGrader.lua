local LevelGrader = {
	expToNextLevel= nil,	-- level -> exp
	fixedPromote  = nil,	-- level -> property inc
	randomPromote = nil,	-- level -> property inc + random
	skillsToLearn = nil,	-- level -> skills
}
LevelGrader.__index = LevelGrader

function LevelGrader.new(expToNextLevel, skillsToLearn)
	assert(type(expToNextLevel)== 'table')
	assert(type(skillsToLearn) == 'table')

	return setmetatable({
			expToNextLevel= expToNextLevel,
			skillsToLearn = skillsToLearn
		}, LevelGrader)
end

function LevelGrader:getNextLevelExp(currentLevel)
	assert(currentLevel >= 1)
	return self.expToNextLevel[currentLevel]
end

function LevelGrader:promote(warrior, atLevel)
	local skillList = self.skillsToLearn[atLevel]
	if skillList then
		
	end
end

return LevelGrader