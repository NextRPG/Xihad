local Skill = require 'Skill.Skill'
local Location = require 'route.Location'
local Equation = require 'Warrior.Equation'
local BuffEffect = require 'Effect.BuffEffect'
local CastableRange = require 'route.CastableRange'
local BuffResolver  = require 'Skill.BuffResolver'
local HitPointResolver = require 'Skill.HitPointResolver'
local RelativeLocationParser   = require 'Skill.RelativeLocationParser'
local P2PParticleSkillAnimator = require 'Skill.P2PParticleSkillAnimator'
local P2TParticleSkillAnimator = require 'Skill.P2TParticleSkillAnimator'

local SkillRegistry = {
	allSkills = {},
	parser = RelativeLocationParser.new(),
}

function SkillRegistry.newSkill(name, range, filter, effecFile, magicAnim)
	local Animator = P2PParticleSkillAnimator
	if filter.toVacancy then
		Animator = P2TParticleSkillAnimator
	end
	
	local animator = Animator.new(effecFile, magicAnim)
	local skill = Skill.new(name, range, filter, animator)
	SkillRegistry.allSkills[name] = skill
	
	return skill
end

function SkillRegistry.toRange(minDst, maxDst, mask)
	local range = CastableRange.new(minDst, maxDst)
	for x, y in SkillRegistry.parser:parse(mask) do
		range:addRelativeImpactLocation(Location.new(x, y))
	end
	
	return range
end


local toRange = SkillRegistry.toRange
local range = toRange(2, 2, '@')

local enemyOnly = { toEnemy = true, }
local fireSkill = SkillRegistry.newSkill('Fire', range, enemyOnly, 'Assets.effect.tornado')
fireSkill:addResolver(HitPointResolver.new('fire', -30))
fireSkill:addResolver(BuffResolver.new(BuffEffect.new('ATK', Equation.new(-10), 2), 1.0))

SkillRegistry.allSkills['skill1'] = Skill.new('skill1', range, enemyOnly)
SkillRegistry.allSkills['skill2'] = Skill.new('skill2', range, enemyOnly)
SkillRegistry.allSkills['skill3'] = Skill.new('skill3', range, enemyOnly)
SkillRegistry.allSkills['skill4'] = Skill.new('skill4', range, enemyOnly)
SkillRegistry.allSkills['skill5'] = Skill.new('skill5', range, enemyOnly)

function SkillRegistry.findSkillByName(name)
	return SkillRegistry.allSkills[name]
end

return SkillRegistry
