local Table = require 'std.Table'
local LevelGrader = require 'Warrior.LevelGrader'
local LevelGraderBuilder = {
	expToNextLevel= nil,	-- level -> exp
	skillsToLearn = nil,	-- level -> skills
}
LevelGraderBuilder.__index = LevelGraderBuilder

function LevelGraderBuilder.new()
	local o = setmetatable({}, LevelGraderBuilder)
	o:_init()
	return o
end

function LevelGraderBuilder:_init()
	self.expToNextLevel= {}
	self.skillsToLearn = {}
end

function LevelGraderBuilder:setExpToNextLevel(exp)
	table.insert(self.expToNextLevel, exp)
	return #self.expToNextLevel
end

function LevelGraderBuilder:addSkillAtLevel(level, skill)
	assert(type(level) == 'number' and skill ~= nil)
	
	local list = Table.get_or_new_table(self.skillsToLearn, level)
	table.insert(list, skill)
end

function LevelGraderBuilder:create()
	local grader = LevelGrader.new(self.expToNextLevel, self.skillsToLearn)
	self:_init()
	return grader
end

return LevelGraderBuilder