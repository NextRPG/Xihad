
---
-- 包括Character的属性，状态，技能，装备，
-- 职业和等级等属性的储存和查询和更新
-- @module Charcter
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Equation = require "Equation"
local Chessboard = require "Chessboard"
local Publisher = require "Publisher"
local Set = require "Set"

---
-- @string team 
-- @int career
-- @int level
-- @int currentExp
-- @int exp 杀掉的时候获得的经验值大小
-- @string name 人物的名字
-- @tparam tab properties 人物的属性集合
-- @tparam tab states 人物所处状态的集合
-- @tparam tab effects 人物所中的效果的集合
-- @tparam tab equipments 人物所穿的装备的集合
-- @tparam tab skills 人物所拥有的技能的集合

local Character = {
	career = 0,
	level = 0,
	currentExp = 0,
	exp = 0,
	name = "",
	skills = {},
	effects = Set.new{},
	equipments = {}, -- Consts.parts
	properties = {},
	states = {TURNOVER = false},
}


---
-- 创建新的Character
-- @tab o
-- @treturn Character o
function Character.new( o )
	assert(type(o) == "table", "prototype must be a table")
	inherit(o, Publisher, Character)

	for k,property in pairs(Consts.properties) do
		-- test 
		o.properties[property] = Equation.new{offset = o.properties[property]}
	end
	o.properties.currentHP = o.properties.maxHP:calculate()
	o.properties.currentAP = o.properties.maxAP:calculate()

	return o
end

--- 
-- 找到当前Character所在的TileObject
-- @treturn Object tileObject
function Character:tile()
	return Chessboard:tileAt(vector2point(self.object:getTranslation()))
end

function Character:getManager(  )
	return self.team == "Hero" and require("HeroManager") or require("AIManager")
end

function Character:getEnemyManager(  )
	return self.team == "Hero" and require("AIManager") or require("HeroManager")  
end

-- EffectTarget
-- unused
-- function Character:bindEffect( effect )
-- 	assert(effect, "effect can't be nil")
-- 	self.effects[effect] = #self.effects
-- end

-- function Character:unbindEffect( effect )
-- 	assert(effect, "effect can't be nil")
-- 	self.effects[effect] = nil	
-- end

-- function Character:updateEffects(  )
-- 	for k,v in pairs(self.effects) do
-- 		k:roundUpdate(self)
-- 	end
-- end

-- function Character:resetEffects(  )
-- 	self.effects = {}
-- end

-- EquipTarget
-- unused

function Character:equip( equipment )
	if equipment:equip(self) then
		self.equipments[#self.equipments + 1] = equipment
	end
end

function Character:unequip( equipment )
	 
end

-- PropertyHost

---
-- 得到某一名字的属性
-- @string name
-- @return int
function Character:getProperty( name )
	if type(self.properties[name]) == "table" then
		return self.properties[name]:calculate()
	else
		return self.properties[name]
	end
end

function Character:levelUp( level )
	-- body
end

function Character:handleDamage_aux( param )
	local attack, defense, skillPower = param.attack, param.defense, param.skillPower
	local damage = math.floor(
	(skillPower * attack*3)/(defense + attack)*(1.1 - math.random() * 0.2))
	print("cause" .. damage .. "damage")
	self:changeState("properties.currentHP", self:getProperty("currentHP") - damage) 
	if (self:getProperty("currentHP") < 0) then
		self.properties.currentHP = 0
		-- dispatch character dead message
	end
end

function Character:handleNormalDamage(attacker)
	self:handleDamage_aux{
		attack = attacker:getProperty("physicalAttack"), 
		defense = self:getProperty("physicalDefense"),
		skillPower = attacker:getProperty("physicalAttack")
	}
end

function Character:handlePhysicalSkill( skill, attacker )
	self:handleDamage_aux{
		attack = attacker:getProperty("physicalAttack"), 
		defense = self:getProperty("physicalDefense"),
		skillPower = skill.damage	
	}
end

function Character:handleMagicSkill( skill, attacker )
	self:handleDamage_aux{
		attack = attacker:getProperty("magicAttack"), 
		defense = self:getProperty("magicDefense"),
		skillPower = skill.damage
	}
end

---
-- 处理各种属性的攻击，进行伤害值计算
-- @tparam Skill skill
-- @tparam Character attacker 
-- @string property 
function Character:handleDamage( skill, attacker, property )
	if     property == "P" then self:handlePhysicalSkill(skill, attacker) 
	elseif property == "M" then self:handleMagicSkill(skill, attacker)
	elseif property == "N" then self:handleNormalDamage(attacker)
	end
end

function Character:handleHeal( heal )
	-- 需要公式
end

return Character