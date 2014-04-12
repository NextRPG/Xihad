--- 
-- 控制相机的移动，同时进行translate和target的移动
-- @module CameraMoveBy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Ease   = require "EaseFunction"
local MoveBy = require "MoveBy"

local CameraMoveBy = {}
CameraMoveBy.__index = CameraMoveBy
setmetatable(CameraMoveBy, MoveBy)

function CameraMoveBy.new( o )
	o = o or {}
	setmetatable(o, CameraMoveBy)

	return o
end

function CameraMoveBy:moveToCharacter( characterObject, callback )
	if self.enabled then return false end

	local action = {}
	action.destTarget = characterObject:getTranslate() + math3d.vector(0, 10, 0)
	action.destination = action.destTarget + math3d.vector(0, 0, 30)
	
	self:runAction(action, callback)
end

function CameraMoveBy:runActionByDelta( action, callback )
	if self.enabled then return false end
	self.delta = action.destination - self.object:getTranslate()
	action.interval = self.delta:length() * 0.01
	self:runAction(action, callback)
end

function CameraMoveBy:runAction( action, callback )
	if self.enabled then return false end

	local ccom = self.object:findComponent(c"Camera")
	self.source2 = ccom:getTarget()
	self.destTarget = action.destTarget or self.source2
	self.originLookDir = self.source2 - self.object:getTranslate()
	self.quaternion = math3d.quaternion(
		self.originLookDir,
		self.destTarget - action.destination)
	self:runActionByInterval(action, callback)

end

function CameraMoveBy:onUpdate(  )
	if not self.enabled then return false end


	local ccom = self.object:findComponent(c"Camera")

	self.leftTime = self.leftTime - g_time.change
	if (self.leftTime <= 0) then
		self.object:resetTranslate(self.destination)
		ccom:setTarget(self.destTarget)
		self.enabled = false
		
		self.callback()
	else
		local newTrans = math3d.lerp(
			self.source, self.destination, Ease.linear(self.leftTime / self.interval))
		local newTarget = math3d.lerp(math3d.quaternion(), self.quaternion,
		 1 - Ease.linear(self.leftTime / self.interval)) * self.originLookDir + newTrans

		ccom:setTarget(newTarget)
		self.object:resetTranslate(newTrans)
	end

	return true
end

return CameraMoveBy