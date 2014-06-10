local Location = require 'route.Location'
local Algorithm= require 'std.Algorithm'
local Coordinate2D = {
	xAxis = Location.new(1, 0),
	yAxis = Location.new(0, 1),
}
Coordinate2D.__index = Coordinate2D

function Coordinate2D.new(xAxis, yAxis)
	return setmetatable({
			xAxis = xAxis,
			yAxis = yAxis,
		}, Coordinate2D)
end

function Coordinate2D.build8Directions_YAxis(yDir)
	local x, y = yDir:xy()
	local nx, ny = Algorithm.sign(x), Algorithm.sign(y)
	
	if math.abs(x) > math.abs(y) then
		ny = 0
	elseif math.abs(x) < math.abs(y) then
		nx = 0
	end
	
	return Location.new(nx, ny)
end

function Coordinate2D.build8Directions(yDir)
	local yAxis = Coordinate2D.build8Directions_YAxis(yDir)
	local xAxis = Location.new(yAxis.y, -yAxis.x)
	return Coordinate2D.new(xAxis, yAxis)
end

function Coordinate2D:transform(location)
	return self.xAxis * location.x + self.yAxis * location.y
end

function Coordinate2D:isStandard()
	return self.xAxis == Coordinate2D.xAxis and self.yAxis == Coordinate2D.yAxis
end

return Coordinate2D