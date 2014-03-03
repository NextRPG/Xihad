RotateBy = {
	destination = {y = 0},
	leftTime = 0,
	enabled = false
}
-- package.loaded[ "RotateBy" ] = RotateBy

package.path = package.path .. ";../?.lua"

function RotateBy.new( o )
	o = o or {}
	setmetatable(o, {__index = RotateBy})
	return o
end

function RotateBy:runAction( action, callback )
	if enabled then return end
	
	local destination = action.destination
	local callback = callback or action.callback 
	local interval = action.interval

	self.destination = destination or { y = 0 }
	self.callback = callback or function ( ) end

	self.leftTime = interval or 1.0
	self.enabled = true
	-- print( " 5 " , coroutine.running())


end

function RotateBy:onUpdate(  )
	if not self.enabled then return end

	local animate = self.object:findComponent(c"AnimatedMesh")	
	local lastFrameTime = Time.change	
	local deltaY = self.destination.y * (lastFrameTime / self.leftTime)
	self.destination.y = self.destination.y - deltaY

	self.object:concatRotate(math3d.vector(0,deltaY,0))

	self.leftTime = self.leftTime - lastFrameTime
	if (self.leftTime <= 0) then
		self.enabled = false
		animate:playAnimation(c"idle 1")
		self.callback()
	end
end

return RotateBy