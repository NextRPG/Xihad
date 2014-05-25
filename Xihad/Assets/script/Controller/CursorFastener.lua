local FixCursorModifier = require 'Modifier.FixCursorModifier'
local CursorFastener = {
	fixPoint = nil,
}
CursorFastener.__index = CursorFastener

function CursorFastener.new()
	return setmetatable({}, CursorFastener)
end

function CursorFastener:onIntersection(intersection)
	self.fixPoint = intersection
end

function CursorFastener:createFixCursorModifier()
	if self.fixPoint then
		return FixCursorModifier.new(self.fixPoint)
	end
end

return CursorFastener