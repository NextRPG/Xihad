-- logic Move, not the pixel Move 
MoveBy = {
	destination = {x = 0, y = 0},
	leftTime = 0,
	enabled = false
}

package.path = package.path .. ";../?.lua"
local m3d = require "math3d"

function MoveBy.new( o )
	o = o or {}

	setmetatable(o, {__index = MoveBy})
	return o
end

local function playAnimation( object, name )
	local animate = object:findComponent(c"AnimatedMesh")
	if animate ~= nil then
		animate:playAnimation(c(name))
	end
end

function MoveBy:runAction( action )
	if self.enabled then return end

	print(action.destination.x, action.destination.y)
	self.source = m3d.vector(self.object:getTranslate())
	self.destination = self.source + 
		m3d.vector(action.destination.x * Consts.TILE_WIDTH, 0, 
			action.destination.y * Consts.TILE_HEIGHT)
	self.callback = action.callback or function ( ) end
	self.interval = action.interval or 0.7
	self.leftTime = self.interval

	playAnimation(self.object, "walk")

	self.enabled = true
end

function MoveBy:onUpdate(  )
	if not self.enabled then return end

	self.leftTime = self.leftTime - Time.change
	if (self.leftTime <= 0) then
		self.object:resetTranslate(self.destination:xyz())
		self.enabled = false
		playAnimation(self.object, "idle 1")
		self.callback()
	else
		self.object:resetTranslate(m3d.lerp(
			self.source, self.destination, self.leftTime / self.interval ):xyz() )
	end

end

return MoveBy