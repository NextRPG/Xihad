local ObjectAction = require 'ObjectAction'
local Trigonometry = require 'math.Trigonometry'
local ActionAdapter= require 'Action.ActionAdapter'
local SpanVariable = require 'Action.SpanVariable'
local WarriorMovement = require 'WarriorMovement'
local AsConditionFactory = require 'Action.AsConditionFactory'

local CommandExecutor = {
	cameraControl = nil,
}
CommandExecutor.__index = CommandExecutor

function CommandExecutor.new(cameraControl)
	return setmetatable({
			cameraControl = cameraControl
		}, CommandExecutor)
end

function CommandExecutor:move(object, destination)
	local action = WarriorMovement.moveToLocation(object, destination)
	if action then
		local animator = object:findComponent(c'AnimatedMesh')
		animator:playAnimation(c'walk')
		AsConditionFactory.waitAction(action)
		animator:playAnimation(c'idle')
	end
end

function CommandExecutor:cast(warrior, skill, target)
	-- TODO 面向 target 位置
	print(string.format('%s cast %s @%s', warrior:getHostObject():getID(), skill, tostring(target)))
	local object = warrior:getHostObject()
	local translate = object:getTranslate()
	
	local targetVector = g_chessboard:getTile(target):getCenterVector()
	local sightLine = targetVector - translate
	local x, _, z = sightLine:xyz()
	local rotation = Trigonometry.toDegree(math.atan2(x, z)) -- x is the logic y, z is the logic x
	local action = ObjectAction.rotateY(object, SpanVariable.new(nil, rotation), 90/0.1)
	ActionAdapter.fit(object, action)
	AsConditionFactory.waitAction(action)
	
	warrior:deactivate()
end

function CommandExecutor:standBy(warrior)
	warrior:deactivate()
end

function CommandExecutor:execute(cmdList)
	local warrior = cmdList:getSource()
	local object = warrior:getHostObject()
	
	self.cameraControl:focus(object)
	
	self:move(object, cmdList:getLocation())
	
	if cmdList:getCommand() == '待机' then
		self:standBy(warrior)
	else
		self:cast(warrior, cmdList:getCommand(), cmdList:getTarget())
	end
end

return CommandExecutor
