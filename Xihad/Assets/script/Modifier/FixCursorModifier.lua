local base = require 'Modifier.Modifier'
local Algorithm = require 'std.Algorithm'
local FixCursorModifier = {}

function FixCursorModifier.create(point)
	local mod = base.new()
	mod.point = point
	
	function mod:onUpdate(time)
		if not self.point then return end
	
		local nowX, nowY = g_collision:getScreenCoordFromPosition(point)
		local maxX, maxY = g_window:getScreenSize()
		
		local clampedX = Algorithm.clamp(nowX, 0, maxX)
		local clampedY = Algorithm.clamp(nowY, 0, maxY)
		
		local prevX, prevY = g_cursor:getPosition()
		g_cursor:setPosition(clampedX, clampedY)
	end
	
	return mod
end

return FixCursorModifier
