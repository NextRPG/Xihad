local Skill = require 'Skill.Skill'
local Location = require 'route.Location'
local CastableRange = require 'route.CastableRange'
local HitPointResolver = require 'Skill.HitPointResolver'
local P2PParticleSkillAnimator = require 'Skill.P2PParticleSkillAnimator'

local SkillRegistry = {
	allSkills = {}
}

local range = CastableRange.new(2, 2)
range:addRelativeImpactLocation(Location.new(0, 0))

local filter = { toEnemy = true, }

local fireSkill = Skill.new('Fire', range, filter, P2PParticleSkillAnimator.new('Assets.effect.fire'))
fireSkill:addResolver(HitPointResolver.new('fire', 30))
SkillRegistry.allSkills['Fire']	  = fireSkill
SkillRegistry.allSkills['skill1'] = Skill.new('skill1', range, filter)
SkillRegistry.allSkills['skill2'] = Skill.new('skill2', range, filter)
SkillRegistry.allSkills['skill3'] = Skill.new('skill3', range, filter)
SkillRegistry.allSkills['skill4'] = Skill.new('skill4', range, filter)
SkillRegistry.allSkills['skill5'] = Skill.new('skill5', range, filter)

function SkillRegistry.findSkillByName(name)
	return SkillRegistry.allSkills[name]
end

return SkillRegistry
