local Skill = require 'Skill.Skill'
local Location = require 'route.Location'
local Equation = require 'Warrior.Equation'
local BuffEffect = require 'Effect.BuffEffect'
local SleepEffect = require 'Effect.SleepEffect'
local HitPointEffect= require 'Effect.HitPointEffect'
local RoundRecycler = require 'Effect.RoundRecycler'
local EffectResolver= require 'Skill.EffectResolver'
local RepelResolver = require 'Skill.RepelResolver'
local HitPointResolver = require 'Skill.HitPointResolver'
local RelativeLocationParser   = require 'Skill.RelativeLocationParser'
local P2PParticleSkillAnimator = require 'Skill.P2PParticleSkillAnimator'
local P2TParticleSkillAnimator = require 'Skill.P2TParticleSkillAnimator'

local OrderedSet= require 'std.OrderedSet'
local CastRange = require 'route.CastRange'
local ProjectiveImpactRange = require 'route.ProjectiveImpactRange'
local OrientationImpactRange= require 'route.OrientationImpactRange'
local SynthesizeRange = require 'route.SynthesizeRange'

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

local function addRow(mask, row)
	mask = mask and mask..'\n' or ''
	return mask..row
end

local function toRange(minDst, maxDst, mask, ImpactType)
	local castRange = CastRange.new(minDst, maxDst)
	
	ImpactType = ImpactType or ProjectiveImpactRange
	local set = OrderedSet.new()
	for x, y in SkillRegistry.parser:parse(mask) do
		set:insert(Location.new(x, y))
	end
	local impactRange = ImpactType.new(set)
	
	return SynthesizeRange.new(castRange, impactRange)
end

local function addEffectResolver(skill, effect, probability)
	skill:addResolver(EffectResolver.new(effect, probability))
end

local function addSleepEffect(skill, round, probability)
	local effect = SleepEffect.new(RoundRecycler.new(round))
	addEffectResolver(skill, effect, probability)
end

local function addPoisonEffect(skill, incr, round, probability)
	local effect = HitPointEffect.new(incr, RoundRecycler.new(round))
	addEffectResolver(skill, effect, probability)
end

local function addBuffEffect(skill, field, eq, round, probability)
	local effect = BuffEffect.new(field, eq, RoundRecycler.new(round))
	addEffectResolver(skill, effect, probability)
end

local function addOffsetBuff(skill, field, offset, round, probability)
	local eq = Equation.new(offset)
	addBuffEffect(skill, field, eq, round, probability)
end

local function addPowerBuff(skill, field, power, round, probability)
	local eq = Equation.new(0, power)
	addBuffEffect(skill, field, eq, round, probability)
end

local function addHitPointResolver(skill, nature, incr)
	skill:addResolver(HitPointResolver.new(nature, incr))
end

local mask = nil
-- mask = addRow(mask, 'xox')
-- mask = addRow(mask, 'o@o')
-- mask = addRow(mask, 'xox')
mask = addRow(mask, 'o')
mask = addRow(mask, 'o')
mask = addRow(mask, '@')
local range = toRange(1, 1, mask, OrientationImpactRange)
local enemyOnly = { toEnemy = true, toVacancy = true, }
local fireSkill = SkillRegistry.newSkill('Fire', range, enemyOnly, 'effect.tornado')
addHitPointResolver(fireSkill, 'fire', -30)
addOffsetBuff(fireSkill, 'ATK',  -10, 2, 1.0)
fireSkill:addResolver(RepelResolver.new(false, 3))

function SkillRegistry.findSkillByName(name)
	return SkillRegistry.allSkills[name]
end

return SkillRegistry
