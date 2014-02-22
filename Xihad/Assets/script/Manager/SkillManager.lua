--- 
-- 负责SkillObject的创建，查询，和控制。
-- skillObject的创建实际上是在CharacterManager中进行的
-- @module SkillManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Chessboard = require "Chessboard"
local SkillDatabase = require "SkillDatabase"

---
-- 
local SkillManager = {
	
}

local function sname( name )
	return c("s" .. name)
end

--- 
-- 创建SkillObject
-- @int id
-- @treturn Object skillObject or nil
function SkillManager:createSkill( id )
	assert(SkillDatabase[id], "the skill doesn't exist")
	if scene:findObject(sname(id)) ~= nil then return nil end
	local skillObject = scene:createObject(sname(id))
	skillObject:appendComponent(c"Skill", SkillDatabase[id])
	skillObject:addTag(c"Skill")
	return skillObject
end

---
-- 查询SkillObject
-- @int id
-- @treturn Object skillObject or nil
function SkillManager:getSkill( id )
	return scene:findObject(sname(id))
end

---
-- 展示技能并选择，TODO
-- @tparam Object characterObject 这一轮中选中的人物
local currentCharacter = object
function SkillManager:onShowSkills( object )
	currentCharacter = object
	local character = currentCharacter:findComponent(c"Character")
	-- 显示技能
	print("开始选择技能")
	for i,v in ipairs(character.skills) do
		print(i,v)
	end
	print("K", "待机")
	print("选择你想使用的技能")
end

---
-- 显示能攻击到的范围
-- @string key 输入的键盘键位
local selectSkill = nil
local targetRange = nil
function SkillManager:onSelectSkill( key )
	local character = currentCharacter:findComponent(c"Character")
	selectSkill = scene:findObject(sname(key)):findComponent(c"Skill")
	for k,v in pairs(selectSkill) do
		print(k,v)
	end
	targetRange = selectSkill:getAttackArea(character:tile())
	Chessboard:markArea(targetRange)
end

---
-- 显示技能的打击范围
-- @tparam Object characterObject
local lastObject = nil
local skillRange = nil
function SkillManager:onSelectTarget( object )
	if lastObject == nil or lastObject:getID() ~= object:getID() then
		local tile = object:findComponent(c"Tile")
		if table.contains(targetRange, tile) then
			Chessboard:recoverArea(skillRange)
			Chessboard:markArea(targetRange)
			skillRange = selectSkill:getRange(tile)
			Chessboard:markArea(skillRange, {139, 0, 139}) -- 紫色
		end
		lastObject = object
	end
end

---
-- 发出技能后展示动画，并作出相应逻辑判断
-- selectSkill可以传入
-- @tparam Object tileObject
function SkillManager:onCastSkill( object, skill, characterObject )

	skill = skill or selectSkill
	characterObject = characterObject or currentCharacter

	Chessboard:recoverArea(skillRange)
	Chessboard:recoverArea(targetRange)
	local character = characterObject:findComponent(c"Character")
	local tile = object:findComponent(c"Tile")
	local anim = characterObject:findComponent(c"AnimatedMesh")

	runAsyncFunc(anim.playAnimation, anim, c(skill.animation))
	anim:playAnimation(c"idle 1")

	if targetRange == nil or table.contains(targetRange, tile) then
		skill:trigger(character, tile)
	end
end

return SkillManager