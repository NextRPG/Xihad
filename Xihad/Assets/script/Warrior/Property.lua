local Equation = require 'Warrior.Equation'
local NamedEffects = require 'Warrior.NamedEffects'

-- Battle Module 
local Property = {
	---
	-- Basic states that should be serialized on disk
	basic = 0,
	
	equation = nil,
	
}
Property.__index = Property

function Property.new()
	return setmetatable({
			basic = 0,
			equation = Equation.new(),
			namedEffects = NamedEffects.new(),
		}, Property)
end

function Property:get()
	return math.abs(self.equation:calculate())
end

function Property:addBasic(delta)
	delta = math.max(delta, -self.basic)
	self.basic = self.basic + delta
	self.equation = self.equation + Equation.new(delta)
end

function Property:getBasic()
	return self.basic
end

function Property:addEquation(equation)
	self.equation = self.equation + equation
end

function Property:subEquation(equation)
	self.equation = self.equation - equation
end

return Property
