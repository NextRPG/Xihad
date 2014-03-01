--- 
-- 负责SkillObject的创建，查询，和控制。
-- skillObject的创建实际上是在CharacterManager中进行的
-- @module SkillManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Chessboard = require "Chessboard"
local SkillDatabase = require "SkillDatabase"
local CameraManager = require "CameraManager"

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
	return scene:findObject(sname(id)):findComponent(c"Skill")
end

function SkillManager:getAllAvailableTargets( character )
	local skills = character.skills
	local allTargets = {}
	self.allTargets = {}
	for i,id in ipairs(character.skills) do
		local targets = self:getSkill(id):getPossibleTargets(character:getEnemyManager())
		for i,target in ipairs(targets) do
			if not table.contains(allTargets, target) then
				allTargets[#allTargets + 1] = target
			end
		end
	end
	self.allTargets = allTargets
	return allTargets
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
	for i,id in ipairs(character.skills) do
		if SkillManager:getSkill(id):hasEnemy(character:tile(), character:getEnemyManager()) then
			print(i,id)
		end
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
	-- targetRange = selectSkill:getAvailableTargets(character:tile(), character:getEnemyManager())
	targetRange = selectSkill:getTargetRange(character:tile())
	Chessboard:pushArea(targetRange, "RED")
end

---
-- 显示技能的打击范围
-- @tparam Object characterObject
local lastObject = nil
local skillRange = nil
function SkillManager:onSelectTarget( object )
	if lastObject == nil or lastObject:getID() ~= object:getID() then
		local tile = object:findComponent(c"Tile")
		Chessboard:popArea(skillRange)
		if table.contains(targetRange, tile) then
			skillRange = selectSkill:getAttackArea(tile)
			Chessboard:pushArea(skillRange, "PURPLE") -- 紫色
		elseif skillRange ~= nil then
			skillRange = nil	
		end
		lastObject = object
	end
end

function SkillManager:back2ShowSkill(  )
	Chessboard:popArea(skillRange)
	Chessboard:popArea(targetRange)
	skillRange, targetRange = nil
	self:onShowSkills(currentCharacter)
end

---
-- 发出技能后展示动画，并作出相应逻辑判断
-- selectSkill可以传入
-- @tparam Object tileObject
function SkillManager:onCastSkill( tile, skill, character )

	skill = skill or selectSkill
	local characterObject = (character == nil) and currentCharacter or character.object

	Chessboard:clearAll()
	skillRange = nil
	local character = characterObject:findComponent(c"Character")
	local anim = characterObject:findComponent(c"AnimatedMesh")
	local rotateBy = characterObject:findComponent(c"RotateBy")
	local rx, ry, rz = characterObject:getRotation():xyz()
	local ty = getLogicAngle(math.p_sub(tile, character:tile()))

	runAsyncFunc(rotateBy.runAction, rotateBy, {destination = {y = calRotation( ry, ty )}, interval = 0.2})
	
	CameraManager:move2Battle(characterObject)
	if skill.animation then
		runAsyncFunc(anim.playAnimation, anim, c(skill.animation))
		anim:playAnimation(c"idle 1")
	end
	if targetRange == nil or table.contains(targetRange, tile) then
		skill:trigger(character, tile)
	end
	
	CameraManager:back2Normal()
	character.states.TURNOVER = true
end

return SkillManager