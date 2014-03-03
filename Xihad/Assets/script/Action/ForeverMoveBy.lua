
local MoveBy = require "MoveBy"
local ForeverMoveBy = {
	destination = {x = 0, y = 0},
	speed = {x = 0, y = 0},
	leftTime = 0,
	enabled = false
}

function ForeverMoveBy.new( o )

	o = o or {}
	setmetatable(o, {__index = ForeverMoveBy})

	return o
end

function ForeverMoveBy:runAction( speed )
	if not (speed.x * self.speed.x > 0 and speed.y * self.speed.y > 0) then
		self.speed = speed
	end
	self.enabled = true
end

function ForeverMoveBy:stopAction(  )
	self.enabled = false
end

local k = 0.2
function ForeverMoveBy:onUpdate(  )
	if not self.enabled then return false end

	local ccom = self.object:findComponent(c"Camera")
	local speed = self.speed

	local newTrans = math3d.vector(speed.x * k, 0, speed.y * k)
	-- local newTarget = newTrans - self.object:getTranslate() + ccom:getTarget()
	ccom:setTarget(ccom:getTarget() + newTrans)
	self.object:resetTranslate(self.object:getTranslate() + newTrans)

end

return ForeverMoveBy