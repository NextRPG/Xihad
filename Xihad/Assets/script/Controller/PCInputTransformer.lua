local functional = require 'std.functional'
local MultiClickDetector = require 'Mouse.MultiClickDetector'
local PCInputTransformer = {
	receiver = nil,
	leftDetector = nil,
	rightDetector= nil,
}
PCInputTransformer.__index = PCInputTransformer

-- Event receiver must deepcopy
function PCInputTransformer.new(receiver)
	local o = setmetatable({
		receiver = receiver,
		leftDetector = MultiClickDetector.new(g_scheduler, 'lPressed', 2),
		rightDetector= MultiClickDetector.new(g_scheduler, 'rPressed', 1),
	}, PCInputTransformer)
	
	local leftListener = functional.bindself(o, 'onLeftClick')
	o.leftDetector:addClickTimesListener(1, leftListener)
	o.leftDetector:addClickTimesListener(2, leftListener)
	
	local rightListener= functional.bindself(o, 'onRightClick')
	o.rightDetector:addClickTimesListener(1, rightListener)
	
	return o
end

function PCInputTransformer:onRightClick(detector, times, x, y)
	assert(detector == self.rightDetector and times == 1)
	print('on right click', times)
	self.receiver:onBack()
end

function PCInputTransformer:onLeftClick(detector, times, x, y)
	assert(detector == self.leftDetector)
	assert(times == 1 or times == 2)
	print('on left click: ', times)
	self.receiver:onTouch(x, y, times)
end

function PCInputTransformer:onMouseEvent( e )
	if e.type == 'mouseMoved'and not e.leftPressed and not e.rightPressed then
		self.receiver:onHover(g_cursor:getPosition())
	elseif self.leftDetector:onMouseEvent(e) == 0 then
		-- absorbed
	elseif self.rightDetector:onMouseEvent(e) == 0 then
		-- absorbed
	else-- pass to others
		return 1
	end
	
	return 0
end

return PCInputTransformer
