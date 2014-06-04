local Location = require 'route.Location'
local Algorithm= require 'std.Algorithm'
local Coordinate = {
	xAxis = Location.new(1, 0),
	yAxis = Location.new(0, 1),
}
Coordinate.__index = Coordinate

function Coordinate.new(xAxis, yAxis)
	return setmetatable({
			xAxis = xAxis,
			yAxis = yAxis,
		}, Coordinate)
end

function Coordinate.build8Directions_YAxis(yDir)
	local x, y = yDir:xy()
	local nx, ny = Algorithm.sign(x), Algorithm.sign(y)
	
	if math.abs(x) > math.abs(y) then
		ny = 0
	elseif math.abs(x) < math.abs(y) then
		nx = 0
	end
	
	return Location.new(nx, ny)
end

function Coordinate.build8Directions(yDir)
	local yAxis = Coordinate.build8Directions_YAxis(yDir)
	local xAxis = Location.new(yAxis.y, -yAxis.x)
	return Coordinate.new(xAxis, yAxis)
end

function Coordinate:transform(location)
	return self.xAxis * location.x + self.yAxis * location.y
end

function Coordinate:isStandard()
	return self.xAxis == Coordinate.xAxis and self.yAxis == Coordinate.yAxis
end

return Coordinate