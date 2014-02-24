--- 
-- 控制相机的移动，同时进行translate和target的移动
-- @module CameraMoveBy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local MoveBy = require "MoveBy"
local CameraMoveBy = MoveBy.new{}

function CameraMoveBy.new( o )
	o = o or {}
	setmetatable(o, {__index = CameraMoveBy})

	return o
end

function CameraMoveBy:moveToCharacter( characterObject )
	if self.enabled then return false end

	local action = {}
	action.destination2 = characterObject:getTranslate() + math3d.vector(0, 15, 0)
	action.destination = action.destination2 + math3d.vector(0, 0, 10)
	
	self:runAction(action, callback)

	return true
end

function CameraMoveBy:runAction( action, callback )
	if self.enabled then return false end

	local ccom = self.object:findComponent(c"Camera")
	self.source2 = ccom:getTarget() + math3d.vector(0, 0, 0)
	print("to do", action.destination2:xyz())

	self.destination2 = action.destination2
	print("real", self.destination2:xyz())


	self:runActionByPixel(action, callback)	
	return true
end

function CameraMoveBy:onUpdate(  )
	if not self.enabled then return false end


	local ccom = self.object:findComponent(c"Camera")
	self.leftTime = self.leftTime - Time.change
	if (self.leftTime <= 0) then
		self.object:resetTranslate(self.destination)
		ccom:setTarget(self.destination2)
		self.enabled = false
		playAnimation(self.object, "idle 1")
		self.callback()
	else
		self.object:resetTranslate(math3d.lerp(
			self.source, self.destination, self.leftTime / self.interval))
		ccom:setTarget(math3d.lerp(
			self.source2, self.destination2, self.leftTime / self.interval))

	end

	return true
end

return CameraMoveBy