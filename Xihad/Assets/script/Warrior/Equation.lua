local Equation = {
	power = 1,
	offset= 0,
	fixed = 0,
}
Equation.__index = Equation

function Equation.new(defaultOffset)
	return setmetatable({
			offset = defaultOffset
		}, Equation)
end

function Equation:copy()
	local res = Equation.new()
	res.power = self.power
	res.offset= self.offset
	res.fixed = self.fixed
	
	return res
end

function Equation:__add(other)
	return self:copy():add(other)
end 

function Equation:__sub(other)
	return self:copy():sub(other)
end

function Equation:add(other)
	self.power = self.power + other.power
	self.offset= self.offset+ other.offset
	self.fixed = self.fixed + other.fixed
	return self
end

function Equation:sub(other)
	self.power = self.power - other.power
	self.offset= self.offset- other.offset
	self.fixed = self.fixed - other.fixed
	return self
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