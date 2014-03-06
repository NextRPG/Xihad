--- 
-- 负责存储技能的伤害和治疗值，属性和攻击范围
-- @module Skill
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Chessboard = require "Chessboard"
local HeroManager = require "HeroManager"
local AIManager = require "AIManager"
local PathFinder = require "PathFinder"
local BaseEffect = require "BaseEffect"


---
-- 
-- @string name
-- @int damage
-- @int heal
-- @string description
-- @int maxTimes 技能使用的最大次数
-- @int currentTimes
-- @string property
-- @int minDistance 
-- @int maxDistance
-- @tparam Effect effect2Self
-- @tparam Effect effect2Target
-- @tab range
local Skill = {
	name = "",
	damage = 0,
	heal = 0,
	description = "",
	-- maxTimes = 0,
	property = "",
	minDistance = 1,
	maxDistance = 1,
	consumeMP  = 0,
	effect2Target = nil
}

-- public

---
--
function Skill.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = Skill})

	o.range = o.range or {{x = 0, y = 0}}

	return o
end

---
-- 获得技能攻击点的范围(1)
-- @tparam tab center
-- @treturn tab attackArea
function Skill:getTargetRange( center )
	local attackArea = {}
	for i=center.x - self.maxDistance, center.x + self.maxDistance do
		for j=center.y - self.maxDistance, center.y + self.maxDistance do
			if  math.abs(i - center.x) + math.abs(j - center.y) <= self.maxDistance and 
				math.abs(i - center.x) + math.abs(j - center.y) >= self.minDistance and
			    inbound{x = i, y = j}
			    and Chessboard:tileAt{x = i, y = j}:canPass() then
			    attackArea[#attackArea + 1] = {x = i, y = j}
			end
		end
	end
	return attackArea
end

---
-- 获得可以攻击的范围(2)
-- @tparam tab center
-- @treturn tab range
function Skill:getAttackArea( center )
	local range = {}
	for i,v in ipairs(self.range) do
		local point = math.p_add(v, center)
		if inbound(point) and Chessboard:tileAt(point):canPass() then
			range[#range + 1] = point
		end
	end
	return range
end

function Skill:hasEnemy( center, manager )
	local checked = {}
	local targetRange = self:getTargetRange(center)
	for i,target in ipairs(targetRange) do
		local attackArea = self:getAttackArea(target)
		for i,tile in ipairs(attackArea) do
			if checked[hash(tile)] == nil 
				and manager:getCharacterByLocation(tile) then
				return true
			end
			checked[hash(tile)] = true
		end
	end	
	return false
end

function Skill:getTargets2Num( center, manager )
	local target2Num = {}
	local targetRange = self:getTargetRange(center)
	for i,target in ipairs(targetRange) do
		local attackArea = self:getAttackArea(target)
		target2Num[target] = 0
		for i,tile in ipairs(attackArea) do
			if manager:getCharacterByLocation(tile) then
				target2Num[target] = target2Num[target] + 1
			end
		end
	end
	return target2Num
end

function Skill:getAvailableTargets( center, manager )

	local targets = {}
	local targetRange = self:getTargetRange(center)
	for i,target in ipairs(targetRange) do
		repeat
			if PathFinder:hasTile(target) then break end
			local attackArea = self:getAttackArea(target)
			for i,tile in ipairs(attackArea) do
				if manager:getCharacterByLocation(tile) then
					targets[#targets + 1] = target
					break
				end
			end
		until true
	end
	return targets
end

function Skill:getPossibleTargets( manager )
	local possibleTargets = {}
	for i,point in ipairs(PathFinder) do
		local targets = self:getAvailableTargets(point, manager)
		for i,target in ipairs(targets) do
			if not table.contains(possibleTargets, target) then
				possibleTargets[#possibleTargets + 1] = target
			end
		end
	end
	return possibleTargets
end

function Skill:getBestTarget( center )
	local bestTarget = findMax(self:getTargets2Num(center, HeroManager))
	return bestTarget
end

-- private

local function checkAttack( hero, character )
	if character and hero.team ~= character.team then
		return true
	end 
	return false
end

local function checkHeal( hero, character )
	if character and hero.team == character.team then
		return true
	end
	return false
end

local function checkSkill( hero, skill, character )
	if skill.property == "P" or skill.property == "M" or skill.property == "N" then
		return checkAttack( hero, character )
	elseif skill.property == "H" then
		return checkHeal( hero, character )
	end
	error("don't have that kind of skill")
end


---
-- 技能在某点被触发
-- @tparam Character hero
-- @tparam Tile targetTile
function Skill:trigger( hero, targetTile )
	-- dispatch message about damage heal and Effect 
	-- hero:bindEffect()

	local tile = targetTile
	local range = self:getAttackArea(tile)

	for i,v in ipairs(range) do
		local character = 	HeroManager:getCharacterByLocation(v) 
						or AIManager:getCharacterByLocation(v)
		if checkSkill(hero, self, character) then
			print(character.properties.currentHP)
			if self.effect2Target 
			and BaseEffect.checkAvailable(self.effect2Target, character) 
			then

				local objName = BaseEffect.makeName(self.effect2Target, character)
				scene:createObjectWithComponent(
					objName, self.effect2Target.name,
					table.merge( self.effect2Target, {
						target = character,
						source = hero}))
			end

			if self.damage then 
				character:handleDamage(self, hero, self.property) 
			end
			print(character.properties.currentHP)
		end
	end
	hero:changeState("properties.currentMP",
		 hero:getProperty("currentMP") - self.consumeMP)
	print(hero:getProperty("currentMP"))
	print("attack end")
end

return Skill