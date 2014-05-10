local CursorEventTransformer = {
	leftDragged = false,
	previousX = -1,
	previousY = -1,
	rightPressedX= -1,
	rightPressedY= -1,
}
CursorEventTransformer.__index = CursorEventTransformer

function CursorEventTransformer.new()
	local obj = setmetatable({ }, CursorEventTransformer)
	return obj
end

function CursorEventTransformer:transform(mouseEvent)
	local x, y = g_cursor:getPosition()
	assert(x >= 0 and y >= 0)
	
	if mouseEvent.type == 'lPressed' then
		self.leftDragged = false
		self.previousX, self.previousY = x, y
	elseif mouseEvent.type == 'lUplift' then
		if not self.leftDragged then
			return 'lClicked'
		else
			self.leftDragged = false
		end
	-- elseif mouseEvent.type == 'rPressed' then
	-- 	self.rightPressedX, self.rightPressedY = x, y
	-- elseif mouseEvent.type == 'rUplift' then
	-- 	if self.rightPressedX == x and self.rightPressedY == y then
	-- 		return 'rClicked'
	-- 	end
	elseif mouseEvent.type == 'mouseMoved' then
		if mouseEvent.leftPressed then
			local dx = x - self.previousX
			local dy = y - self.previousY
			
			self.previousX = x
			self.previousY = y
			self.leftDragged = true
			return 'lDragged', dx, dy
		else
			return 'mouseMoved'
		end
	end
end

return CursorEventTransformer