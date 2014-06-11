local Table = require 'std.Table'
local LevelGrader = require 'Warrior.LevelGrader'
local LevelGraderBuilder = {
	nextLevelExpCalculator = nil,	-- level -> exp
	propertyPromoters = nil,
	skillsToLearn = nil,	-- level -> skills
}
LevelGraderBuilder.__index = LevelGraderBuilder

function LevelGraderBuilder.new()
	local o = setmetatable({}, LevelGraderBuilder)
	o:_init()
	return o
end

function LevelGraderBuilder:_init()
	self.nextLevelExpCalculator = nil
	self.propertyPromoters = {}
	self.skillsToLearn = {}
end

function LevelGraderBuilder:setNextLevelExpCalculator(nextLevelExpCalculator)
	self.nextLevelExpCalculator = nextLevelExpCalculator
end

function LevelGraderBuilder:setPropertyPromoter(pname, promoter)
	assert(type(promoter) == 'function')
	self.propertyPromoters[pname] = promoter
end

function LevelGraderBuilder:addSkillAtLevel(level, skill, count)
	assert(type(level) == 'number' and skill ~= nil)
	
	local list = Table.get_or_new_table(self.skillsToLearn, level)
	table.insert(list, { skill = skill, count = count })
end

function LevelGraderBuilder:create()
	local grader = LevelGrader.new(
						self.nextLevelExpCalculator, self.skillsToLearn, 
						self.propertyPromoters)
	
	self:_init()
	return grader
end

return LevelGraderBuilder