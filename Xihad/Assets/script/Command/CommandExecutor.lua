local sCoroutine = require 'std.sCoroutine'
local ObjectAction = require 'HighAction.ObjectAction'
local Trigonometry = require 'math.Trigonometry'
local ActionAdapter= require 'Action.ActionAdapter'
local SpanVariable = require 'Action.SpanVariable'	
local ActionAdapter= require 'Action.ActionAdapter'
local SkillRegistry= require 'Skill.SkillRegistry'
local ConcurrentJobs = require 'std.ConcurrentJobs'
local WarriorMovement = require 'HighAction.WarriorMovement'
local AsConditionFactory = require 'Async.AsConditionFactory'

local CommandExecutor = {
	cameraFacade = nil,
	expCaculator = nil,
	ui = nil,
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

local function getRotationFromSightLine(sightLine)
	local x, _, z = sightLine:xyz()
	
	-- x is the logic y, z is the logic x
	return Trigonometry.toDegree(math.atan2(x, z)) 
end

function CommandExecutor:_faceToTarget(warrior, targetTile)
	local object = warrior:getHostObject()
	local sightLine = targetTile:getCenterVector() - object:getTranslate()
	local rotation = getRotationFromSightLine(sightLine)
	
	local action = ObjectAction.rotateY(object, SpanVariable.new(nil, rotation), 90/0.1)
	ActionAdapter.fit(object, action)
	AsConditionFactory.waitAction(action)
end

function CommandExecutor:_playSkillAnimation(warrior, skill, targetTile, results)
	local current= coroutine.running()
	
	skill:playAnimation(warrior, targetTile, {
			onAttackBegin = function() 
				for _, result in pairs(results) do
					result:onHitBegin()
				end
			end,
			
			onAttackEnd = function ()
				sCoroutine.resume(current)
			end,
		})
	
	coroutine.yield()	-- wait until onAttackEnd
end

function CommandExecutor:_applyBattleResults(results)
	local jobs = ConcurrentJobs.new()
	for _, result in ipairs(results) do
		jobs:addJob(function ()
			result:apply()
			result:onHitEnd()
		end)
	end
	
	jobs:join()
end

function CommandExecutor:_gainExp(warrior, exp)
	-- local level = warrior:findPeer(c'Level')
	-- for usedExp, levelUpInfo in level:obtainExp(exp) do
	-- 	-- wait for some a seconds
	-- 	self.ui:showExpGauge() 
		
	-- 	if levelUpInfo then
	-- 		-- wait for user interactive
	-- 		self.ui:showLevelUpPanel(levelUpInfo)
	-- 	end
	-- end
end

function CommandExecutor:_getBattleResults(warrior, skill, targetLocation)
	local caster = warrior:findPeer(c'SkillCaster')
	return caster:castSkill(skill, targetLocation, g_chessboard)
end

function CommandExecutor:cast(warrior, targetLocation, skill)
	print(string.format('%s cast %s @%s', 
		warrior:getHostObject():getID(), skill:getName(), tostring(targetLocation)))
	
	-- local skill  = SkillRegistry.findSkillByName(skillName)
	local results= self:_getBattleResults(warrior, skill, targetLocation)
	local targetTile = g_chessboard:getTile(targetLocation)
	
	self:_faceToTarget(warrior, targetTile)
	
	self.cameraFacade:descendIntoBattle(targetTile)
	
	self:_playSkillAnimation(warrior, skill, targetTile, results)
	
	self:_applyBattleResults(results)
	
	-- self:_gainExp(warrior, self.expCaculator:calculate())
	
	-- TODO gain item
	
	AsConditionFactory.waitTimer(0.5)
	
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
		self:cast(warrior, cmdList:getTarget(), cmdList:getAsCastable())
	end
	
	-- TODO Wait fade out
end

return CommandExecutor
