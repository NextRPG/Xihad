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
	self.source2 = ccom:getTarget()
	self.destination2 = action.destination2 or self.source2
	self:runActionByPixel(action, callback)	
	return true
end

function CameraMoveBy:onUpdate(  )
	if not self.enabled then return false end


	local ccom = self.object:findComponent(c"Camera")

	print("realTrans", self.object:getTranslate():xyz())
	print("realTarget", ccom:getTarget():xyz())
	self.leftTime = self.leftTime - Time.change
	-- if (self.leftTime <= 0) then
	-- 	self.object:resetTranslate(self.destination)
	-- 	ccom:setTarget(self.destination2)
	-- 	self.enabled = false
	-- 	playAnimation(self.object, "idle 1")
	-- 	self.callback()
	-- else
		-- local newTrans = math3d.lerp(
		-- 	self.source, self.destination, ease.linear(self.leftTime / self.interval))
		-- local newTarget = newTrans - self.source + self.source2

		-- ccom:setTarget(newTarget)

		-- self.object:resetTranslate(newTrans)
		-- self.object:concatTranslate(newTrans - self.object:getTranslate())
		ccom:setTarget(ccom:getTarget() + math3d.vector(0.5, 0, 0))
		self.object:concatTranslate(math3d.vector(0.5, 0, 0))
	-- end

	return true
end

return CameraMoveBy