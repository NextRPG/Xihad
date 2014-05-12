local ObjectAction = require 'ObjectAction'
local Trigonometry = require 'math.Trigonometry'
local ActionAdapter= require 'Action.ActionAdapter'
local SpanVariable = require 'Action.SpanVariable'	
local ActionAdapter= require 'Action.ActionAdapter'
local SkillRegistry= require 'Skill.SkillRegistry'
local WarriorMovement = require 'WarriorMovement'
local AsConditionFactory = require 'Async.AsConditionFactory'

local CommandExecutor = {
	cameraFacade = nil,
}
CommandExecutor.__index = CommandExecutor

function CommandExecutor.new(cameraFacade)
	return setmetatable({
			cameraFacade = cameraFacade
		}, CommandExecutor)
end

function CommandExecutor:move(object, destination)
	local action = WarriorMovement.moveToLocation(object, destination)
	if action then
		ActionAdapter.fit(object, action)
		local animator = object:findComponent(c'AnimatedMesh')
		animator:playAnimation(c'walk')
		AsConditionFactory.waitAction(action)
		animator:playAnimation(c'idle')
	end
end

function CommandExecutor:cast(warrior, skillName, targetLocation)
	print(string.format('%s cast %s @%s', 
		warrior:getHostObject():getID(), skillName, tostring(targetLocation)))
	
	local object = warrior:getHostObject()
	local translate = object:getTranslate()
	
	-- 使角色面朝目标点
	local targetTile = g_chessboard:getTile(targetLocation)
	local targetVector = targetTile:getCenterVector()
	local sightLine = targetVector - translate
	local x, _, z = sightLine:xyz()
	local rotation = Trigonometry.toDegree(math.atan2(x, z)) -- x is the logic y, z is the logic x
	local action = ObjectAction.rotateY(object, SpanVariable.new(nil, rotation), 90/0.1)
	ActionAdapter.fit(object, action)
	AsConditionFactory.waitAction(action)
	
	-- 拉低相机
	self.cameraFacade:descendIntoBattle(targetTile)
	
	-- 发动法术
	local skill = SkillRegistry.findSkillByName(skillName)
	local current = coroutine.running()
	skill:playAnimation(warrior, targetTile, {
			onAttackBegin = function() 
				-- takeDamage() -> playHitAnimation()
				print('receive begin')
				local skillCaster = object:findComponent(c'SkillCaster')
				local results = skillCaster:castSkill(skill, targetLocation, g_chessboard)
				for _, result in ipairs(results) do
					result:apply()
				end
			end,
			
			onAttackEnd = function ()
				print('receive end')
				-- play idle or dead animation
				coroutine.resume(current)
			end,
		})
	
	coroutine.yield()
	
	-- 抬高相机
	self.cameraFacade:ascendAwayBattle()
	warrior:deactivate()
end

function CommandExecutor:standBy(warrior)
	warrior:deactivate()
end

function CommandExecutor:execute(cmdList)
	local warrior= cmdList:getSource()
	
	local object = warrior:getHostObject()
	
	if cmdList:getLocation() == warrior:getLocation() and cmdList:getCommand() == '待机' then
		self:standBy(warrior)
		-- TODO Don't wait fade out
		return
	end
	
	self.cameraFacade:focus(object)
	
	self:move(object, cmdList:getLocation())
	
	if cmdList:getCommand() == '待机' then
		self:standBy(warrior)
	else
		self:cast(warrior, cmdList:getCommand(), cmdList:getTarget())
	end
	
	-- TODO Wait fade out
end

return CommandExecutor
