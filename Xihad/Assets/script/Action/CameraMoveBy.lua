
local MoveBy = require "MoveBy"
local CameraMoveBy = {
	destination = {x = 0, y = 0},
	leftTime = 0,
	enabled = false
}

function CameraMoveBy.new( o )

	o = o or {}
	setmetatable(o, {__index = CameraMoveBy})

	return o
end

function CameraMoveBy:runAction( action )
	if self.enabled then return false end

	local k = 1

	local ccom = self.object:findComponent(c"Camera")
	self.source2 = ccom:getTarget()
	self.destination2 = self.source2 + 
	math3d.vector(action.destination.x * k, 0, 
		action.destination.y * k)
		self.source = self.object:getTranslate()

	self.destination = self.source + 
		math3d.vector(action.destination.x * k, 0, 
			action.destination.y * k)
	self.callback = action.callback or function ( ) end
	self.interval = action.interval or 0.1
	self.leftTime = self.interval

	self.enabled = true
end

function CameraMoveBy:onUpdate(  )
	if not self.enabled then return false end

	local ccom = self.object:findComponent(c"Camera")
	self.leftTime = self.leftTime - Time.change

	if (self.leftTime <= 0) then
		ccom:setTarget(self.destination2)
		self.object:resetTranslate(self.destination)
		self.enabled = false
		self.callback()
	else
		-- ccom:setTarget(math3d.lerp(
		-- 	self.source2, self.destination2, self.leftTime / self.interval))
		local newTrans = math3d.lerp(self.source, self.destination, self.leftTime / self.interval)
		local newTarget = newTrans - self.object:getTranslate() + ccom:getTarget()
		ccom:setTarget(newTarget)
		self.object:resetTranslate(newTrans)
	end

	return true
end

return CameraMoveBy