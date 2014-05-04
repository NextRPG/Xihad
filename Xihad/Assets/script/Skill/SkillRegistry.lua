local Skill = require 'Skill.Skill'
local Location = require 'route.Location'
local CastableRange = require 'route.CastableRange'

local SkillRegistry = {
	allSkills = {}
}

local range = CastableRange.new(2, 2)
range:addRelativeImpactLocation(Location.new(-1, 0))
range:addRelativeImpactLocation(Location.new(1, 0))
range:addRelativeImpactLocation(Location.new(0, 1))
range:addRelativeImpactLocation(Location.new(0, -1))
SkillRegistry.allSkills['Fire'] = Skill.new('Fire', range)
SkillRegistry.allSkills['skill1'] = Skill.new('skill1', range)
SkillRegistry.allSkills['skill2'] = Skill.new('skill2', range)
SkillRegistry.allSkills['skill3'] = Skill.new('skill3', range)
SkillRegistry.allSkills['skill4'] = Skill.new('skill4', range)
SkillRegistry.allSkills['skill5'] = Skill.new('skill5', range)

function SkillRegistry.findSkillByName(name)
	return SkillRegistry.allSkills[name]
end

return SkillRegistry
