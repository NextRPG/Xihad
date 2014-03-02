--- 
-- 负责存储技能的伤害和治疗值，属性和攻击范围
-- @module Skill
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Chessboard = require "Chessboard"
local HeroManager = require "HeroManager"
local AIManager = require "AIManager"

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
	maxTimes = 0,
	property = "",
	minDistance = 1,
	maxDistance = 1
}

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



-- public

---
--
function Skill.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = Skill})

	o.effect2Self = o.effect2Self or nil
	o.effect2Target = o.effect2Target or nil
	o.range = o.range or {{x = 0, y = 0}}

	o.currentTimes = o.maxTimes
	return o
end

---
-- 获得可以攻击的范围(1)
-- @tparam tab center
-- @treturn tab attackArea
function Skill:getAttackArea( center )
	local attackArea = {}
	for i=center.x - self.maxDistance, center.x + self.maxDistance do
		for j=center.y - self.maxDistance, center.y + self.maxDistance do
			if  math.abs(i - center.x) + math.abs(j - center.y) <= self.maxDistance and 
				math.abs(i - center.x) + math.abs(j - center.y) >= self.minDistance and
			    i >= 0 and i < Consts.COLS and j >= 0 and j < Consts.ROWS 
			    and Chessboard:tileAt{x = i, y = j}:findComponent(c"Tile"):canPass() then
			    attackArea[#attackArea + 1] = {x = i, y = j}
			end
		end
	end
	return attackArea
end

---
-- 获得技能攻击点的范围(2)
-- @tparam tab center
-- @treturn tab range
function Skill:getRange( center )
	local range = {}
	for i,v in ipairs(self.range) do
		range[#range + 1] = {x = v.x + center.x, y = v.y + center.y}
	end
	return range
end

function Skill:hasEnemy( center )
	local checked = {}
	local attackArea = self:getAttackArea(center)
	for i,target in ipairs(attackArea) do
		local range = self:getRange(target)
		for i,tile in ipairs(range) do
			if checked[tile.x .. " " .. tile.y] == nil 
				and HeroManager:getCharacterByLocation(tile) then
				return true
			end
			checked[tile.x .. " " .. tile.y] = true
		end
	end	
	return false
end

function Skill:getBestTarget( center )
	local target2Num = {}
	local attackArea = self:getAttackArea(center)
	for i,target in ipairs(attackArea) do
		local range = self:getRange(target)
		target2Num[target] = 0
		for i,tile in ipairs(range) do
			if HeroManager:getCharacterByLocation(tile) then
				target2Num[target] = target2Num[target] + 1
			end
		end
	end
	local bestTarget = findMax(target2Num)
	return bestTarget
end

---
-- 技能在某点被触发
-- @tparam Character hero
-- @tparam Tile targetTile
function Skill:trigger( hero, targetTile )
	-- dispatch message about damage heal and Effect 
	-- hero:bindEffect()

	local tile = targetTile
	local range = self:getRange(tile)

	for i,v in ipairs(range) do
		local object = 	HeroManager:getCharacterByLocation(v) 
						or AIManager:getCharacterByLocation(v)
		local character
		if object then
			character = object:findComponent(c"Character")
		end
		if checkSkill(hero, self, character) then
			print(character.properties.currentHP)
			if self.effect2Target then character:bindEffect(self.effect2Target) end
			if self.damage then character:handleDamage(self, hero, self.property) end
			print(character.properties.currentHP)
		end
	end
	print("attack end")
end

return Skill