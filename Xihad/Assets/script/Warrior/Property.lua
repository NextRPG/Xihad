local Equation = require 'Warrior.Equation'

-- Battle Module 
local Property = {
	---
	-- Basic states that should be serialized on disk
	basic = 0,
	
	equation = nil,
	
	--- 
	-- Temporary states those generated in a battle
	namedStates = nil,
}
Property.__index = Property

function Property.new()
	return setmetatable({
			basic = 0,
			equation = Equation.new(),
			namedStates = {},
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

function Property:attach(type, state)
	self:_detach_no_check(type)
	
	self.equation = self.equation + state:getEquation()
	self.namedStates[type] = state
	state:onAttached()
end

function Property:_detach_no_check(type)
	local attached = self.namedStates[type]
	
	if attached then
		self.equation = self.equation - attached:getEquation()
		self.namedStates[type] = nil
	
		attached:onDetached()
	end
end

function Property:detach(type, state)
	local attached = self.namedStates[type]
	
	if attached == state then
		self:_detach_no_check(type)
	end
end

function Property:allAttachedStates()
	return pairs(self.namedStates)
end

return Property
