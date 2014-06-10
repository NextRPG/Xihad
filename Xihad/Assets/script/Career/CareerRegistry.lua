local Career = require 'Career.Career'
local SkillRegistry = require 'Skill.SkillRegistry'
local LevelGraderBuilder = require 'Warrior.LevelGraderBuilder'
local CareerRegistry = {
	datas = {},
}

function CareerRegistry.findCareerByName(name)
	return CareerRegistry.datas[name]
end

function CareerRegistry.addCareer(name, grader)
	CareerRegistry.datas[name] = Career.new(name, grader)
end

local addCareer = CareerRegistry.addCareer
local builder = LevelGraderBuilder.new()
builder:setNextLevelExpCalculator(function (level)
	return math.floor(0.1*math.pow(level, 2.5) + level*level + 20*level)
end)

builder:setPropertyPromoter('ATK', function (warrior, level)
	return math.floor(math.log(level) / math.log(10) + 1)
end)

builder:addSkillAtLevel(2, SkillRegistry.findSkillByName('Fire'), 20)
CareerRegistry.addCareer('WARRIOR', builder:create())

return CareerRegistry
