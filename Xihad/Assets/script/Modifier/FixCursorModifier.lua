local base = require 'Modifier.Modifier'
local Algorithm = require 'std.Algorithm'
local FixCursorModifier = setmetatable({
	point = nil,
}, base)
FixCursorModifier.__index = FixCursorModifier

function FixCursorModifier.new(point)
	local obj = setmetatable(base.new(), FixCursorModifier)
	obj.point = point:copy()
	return obj
end

function FixCursorModifier:onUpdate(time)
	if not self.point then return end

	local nowX, nowY = g_collision:getScreenCoordFromPosition(self.point)
	local maxX, maxY = g_window:getScreenSize()
	
	local clampedX = Algorithm.clamp(nowX, 0, maxX)
	local clampedY = Algorithm.clamp(nowY, 0, maxY)
	
	local prevX, prevY = g_cursor:getPosition()
	g_cursor:setPosition(clampedX, clampedY)
end

return FixCursorModifier
