local base = require 'Action.SpanVariable'
local SpanColor = setmetatable({}, base)
SpanColor.__index = SpanColor

function SpanColor.new(...)
	return setmetatable(base.new(...), SpanColor)
end

function SpanColor:interpolate(d)
	local color1, color2 = self.origin, self.finish
	
	local alpha = color1:getAlpha() * (1-d) + color2:getAlpha() * d
	local red 	= color1:getRed() 	* (1-d) + color2:getRed() 	* d
	local green = color1:getGreen() * (1-d) + color2:getGreen() * d
	local blue 	= color1:getBlue() 	* (1-d) + color2:getBlue() 	* d
	
	return Color.new(alpha, red, green, blue)
end

return SpanColor