local Equation = {
	power = 1,
	offset = 0,
	fixed = 0
}

function Equation.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = Equation})

	return o
end

function Equation:addOffset( inc )
	self.offset = self.offset + inc
end

function Equation:calculate(  )
	return self.power*self.offset + self.fixed
end

function Equation:addPower( inc )
	self.power = self.power + inc
end

function Equation:reducePower( dec )
	self.power = self.power - dec
end

function Equation:addFixed( inc )
	self.fixed = self.fixed + inc
end

function Equation:reduceFixed( dec )
	self.fixed = self.fixed - dec
end

return Equation