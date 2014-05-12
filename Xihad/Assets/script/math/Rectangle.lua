local Rectangle = {
	LB = nil,
	LT = nil,
	RB = nil,
	RT = nil, 
}
Rectangle.__index = Rectangle

function Rectangle.new(lb, lt, rb, rt)
	return setmetatable({
			LB = lb:copy(),
			LT = lt:copy(),
			RB = rb:copy(),
			RT = rt:copy(),
		}, Rectangle)
end

function Rectangle.newXZ(lb, width, height)
	return setmetatable({
			LB = lb:copy(),
			LT = lb + math3d.vector(0, 0, height),
			RB = lb + math3d.vector(width, 0, 0),
			RT = lb + math3d.vector(width, 0, height),
		}, Rectangle)
end

function Rectangle:transform(f)
	self.LB = f(self.LB)
	self.LT = f(self.LT)
	self.RB = f(self.RB)
	self.RB = f(self.RT)
end

function Rectangle:getArea()
	local lowerTri = math3d.triangle(self.LB, self.RB, self.LT)
	local upperTri = math3d.triangle(self.RB, self.RT, self.LT)
	
	return lowerTri:getArea() + upperTri:getArea()
end

return Rectangle