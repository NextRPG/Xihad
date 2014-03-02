 --- 
-- logic Move, not the pixel Move
-- @module MoveBy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local MoveBy = {
	destination = {x = 0, y = 0},
	leftTime = 0,
	enabled = false
}

package.path = package.path .. ";../?.lua"

function MoveBy.new( o )
	
	o = o or {}	
	setmetatable(o, {__index = MoveBy})
	return o
end

function playAnimation( object, name )
	local animate = object:findComponent(c"AnimatedMesh")
	if animate ~= nil then
		animate:playAnimation(c(name))
	end
end

function MoveBy:runAction( action, callback )
	if self.enabled then return false end

	self.source = self.object:getTranslate()
	action.destination = self.source + 
		math3d.vector(action.destination.x * Consts.TILE_WIDTH, 0, 
			action.destination.y * Consts.TILE_HEIGHT)
	self:runActionByInterval(action, callback)

	playAnimation( self.object, "walk" )

	return true
end

function MoveBy:runActionByInterval( action, callback )
	if self.enabled then return false end

	self.source = self.object:getTranslate()
	self.destination = action.destination
	self.callback = callback or function ( ) end
	self.interval = action.interval or 0.5
	self.leftTime = self.interval

	self.enabled = true
	return true
end

function MoveBy:runActionByDelta( action, callback )
	if self.enabled then return false end

	self.source = self.object:getTranslate()
	self.destination = action.destination
	self.callback = callback or function ( ) end
	self.delta = self.destination - self.source
	self.leftDelta = self.delta

	self.enabled = true
	return true
end

function MoveBy:onUpdate(  )
	if not self.enabled then return false end

	self.leftTime = self.leftTime - Time.change
	if (self.leftTime <= 0) then
		self.object:resetTranslate(self.destination)
		self.enabled = false
		playAnimation(self.object, "idle 1")
		self.callback()
	else
		self.object:resetTranslate(math3d.lerp(
			self.source, self.destination, self.leftTime / self.interval))
	end

	return true
end

return MoveBy