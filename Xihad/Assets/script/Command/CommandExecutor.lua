local sCoroutine = require 'std.sCoroutine'
local ObjectAction = require 'HighAction.ObjectAction'
local Trigonometry = require 'math.Trigonometry'
local ActionAdapter= require 'Action.ActionAdapter'
local SpanVariable = require 'Action.SpanVariable'	
local ActionAdapter= require 'Action.ActionAdapter'
local ItemRegistry = require 'Item.ItemRegistry'
local SkillRegistry= require 'Skill.SkillRegistry'
local ConcurrentJobs = require 'std.ConcurrentJobs'
local WarriorMovement= require 'HighAction.WarriorMovement'
local AsConditionFactory = require 'Async.AsConditionFactory'

local CommandExecutor = {
	cameraFacade = nil,
	expCalculator= nil,
	ui = nil,
}
CommandExecutor.__index = CommandExecutor

function CommandExecutor.new(cameraFacade, expCalculator)
	return setmetatable({
			cameraFacade = cameraFacade,
			expCalculator= expCalculator,
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

local function attachRotateAction(object, targetVector)
	local sightLine = targetVector - object:getTranslate()
	local var = SpanVariable.new(nil, getRotationFromSightLine(sightLine))
	local action = ObjectAction.rotateY(object, var, 90/0.1)
	ActionAdapter.fit(object, action)
	return action
end

local function waitRotate(object, targetVector, jobs)
	local action = attachRotateAction(object, targetVector)
	jobs:addJob(function ()
		AsConditionFactory.waitAction(action)
	end)
end

function CommandExecutor:_faceToTarget(warrior, targetTile, results)
	local jobs = ConcurrentJobs.new()
	local object = warrior:getHostObject()
	waitRotate(object, targetTile:getCenterVector(), jobs)
	
	local faceTo = object:getTranslate()
	for barrier, result in pairs(results) do
		if barrier:findPeer(c'Warrior') then
			waitRotate(barrier:getHostObject(), faceTo, jobs)
		end
	end
	
	jobs:join()
end

function CommandExecutor:_playSkillAnimation(warrior, skill, targetTile, results)
	local current = coroutine.running()
	
	skill:playAnimation(warrior, targetTile, {
			onAttackBegin = function() 
				for target, result in pairs(results) do
					result:onHitBegin(warrior, target)
				end
			end,
			
			onAttackEnd = function ()
				sCoroutine.resume(current)
			end,
		})
	
	coroutine.yield()	-- wait until onAttackEnd
end

function CommandExecutor:_applyBattleResults(source, results, lis)
	local jobs= ConcurrentJobs.new()
	for target, result in pairs(results) do
		jobs:addJob(function ()
			result:apply(source, target, lis)
			lis:complete(target)
			
			result:onHitEnd(source, target)
		end)
	end
	
	jobs:join()
end

function CommandExecutor:_gainExp(warrior)
	local leveler = warrior:findPeer(c'Leveler')
	if not leveler then return end
	
	local exp = self.expCalculator:getResult()
	print(warrior:getHostObject():getID(), 'gain exp', exp)
	-- leveler:obtainExp(exp, callback)
end

function CommandExecutor:_getBattleResults(warrior, skill, targetLocation)
	local caster = warrior:findPeer(c'SkillCaster')
	return caster:castSkill(skill, targetLocation, g_chessboard)
end

function CommandExecutor:cast(warrior, targetLocation, skillName)
	print(string.format('%s cast %s @%s', 
		warrior:getHostObject():getID(), skillName, tostring(targetLocation)))
	
	local skill = SkillRegistry.findSkillByName(skillName)
	local results = self:_getBattleResults(warrior, skill, targetLocation)
	local targetTile = g_chessboard:getTile(targetLocation)
	
	-- Done
	self:_faceToTarget(warrior, targetTile, results)
	
	-- Done
	self.cameraFacade:descendIntoBattle(targetTile)
	
	-- Done
	self:_playSkillAnimation(warrior, skill, targetTile, results)
	
	-- Done
	self.expCalculator:clear()
	self:_applyBattleResults(warrior, results, self.expCalculator)
	
	-- Need UI
	self:_gainExp(warrior)
	
	-- TODO gain item
	AsConditionFactory.waitTimer(0.5)
	
	-- Done
	self.cameraFacade:ascendAwayBattle()
	
	warrior:deactivate()
end

function CommandExecutor:useItem(warrior, itemName)
	local item = ItemRegistry.findItemByName(itemName)
	local usage= warrior:findPeer(c'Parcel'):useItem(item)
	
	-- TODO
	print(usage)
	
	AsConditionFactory.waitTimer(0.5)
	if item:occupyRound() then
		warrior:deactivate()
	end
end

function CommandExecutor:standBy(warrior)
	warrior:deactivate()
end

function CommandExecutor:execute(cmdList)
	local warrior= cmdList:getSource()
	local object = warrior:getHostObject()
	local command, subCommand = cmdList:getCommand()
	
	if cmdList:getLocation() == warrior:getLocation() and command == '待机' then
		self:standBy(warrior)
		-- TODO Don't wait fade out
		return
	end
	
	self.cameraFacade:focus(object)
	
	self:move(object, cmdList:getLocation())
	
	if command == '待机' then
		self:standBy(warrior)
	elseif command == '技能' then
		self:cast(warrior, cmdList:getTarget(), subCommand)
	elseif command == '道具' then
		self:useItem(warrior, subCommand)
	end
	
	-- TODO Wait fade out
end

return CommandExecutor
