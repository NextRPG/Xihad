local Equation = {
	power = 1,
	offset = 0,
	fixed = 0
}
Equation.__index = Equation


function Equation.new()
	local o = setmetatable({}, Equation)

	return o
end

function Equation:calculate(  )
	return self.power*self.offset + self.fixed
end

function Equation:incOffset( inc )
	self.offset = self.offset + inc
end

function Equation:incPower( inc )
	self.power = self.power + inc
end

function Equation:incFixed( inc )
	self.fixed = self.fixed + inc
end

return Equation