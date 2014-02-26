--- 
-- 控制相机的移动，同时进行translate和target的移动
-- @module CameraMoveBy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local ease = require "EaseFunction"
local MoveBy = require "MoveBy"
local CameraMoveBy = MoveBy.new{}

function CameraMoveBy.new( o )
	o = o or {}
	setmetatable(o, {__index = CameraMoveBy})

	return o
end

function CameraMoveBy:moveToCharacter( characterObject, callback )
	if self.enabled then return false end

	local action = {}
	action.destination2 = characterObject:getTranslate() + math3d.vector(0, 10, 0)
	action.destination = action.destination2 + math3d.vector(0, 0, 10)
	
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
	self.destination2 = action.destination2 or self.source2
	self.sourceRotate = self.source2 - self.object:getTranslate()
	self.quaternion = math3d.quaternion(
		self.sourceRotate,
		self.destination2 - action.destination)
	self:runActionByInterval(action, callback)

end

function CameraMoveBy:onUpdate(  )
	if not self.enabled then return false end


	local ccom = self.object:findComponent(c"Camera")

	-- print("Time.change", Time.change)
	-- print("realTrans", self.object:getTranslate():xyz())
	-- print("realTarget", ccom:getTarget():xyz())
	-- print("destination2", self.destination2:xyz())
	self.leftTime = self.leftTime - Time.change
	if (self.leftTime <= 0) then
		self.object:resetTranslate(self.destination)
		ccom:setTarget(self.destination2)
		-- playAnimation(self.object, "idle 1")
		print("2 the end")
		self.enabled = false
		self.callback()

	else
		local newTrans = math3d.lerp(
			self.source, self.destination, ease.linear(self.leftTime / self.interval))
		local newTarget = math3d.lerp(math3d.quaternion(), self.quaternion,
		 1 - ease.linear(self.leftTime / self.interval)) * self.sourceRotate + newTrans

		ccom:setTarget(newTarget)
		self.object:resetTranslate(newTrans)
	end

	return true
end

return CameraMoveBy