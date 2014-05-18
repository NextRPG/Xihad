local Equation = require 'Warrior.Equation'
local NamedEffects = require 'Warrior.NamedEffects'

-- Battle Module 
local Property = {
	---
	-- Basic states that should be serialized on disk
	basic = 0,
	
	equation = nil,
	
	--- 
	-- Temporary states those generated in a battle
	namedEffects = nil,
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

function Property:_addEffect(effect)
	self.equation = self.equation + effect:getEquation()
end

function Property:_subEffect(effect)
	self.equation = self.equation - effect:getEquation()
end

function Property:attach(type, effect)
	local detached = self.namedEffects:attach(type, effect)
	
	if effect then
		self:_addEffect(effect)
	end
	
	if detached then
		self:_subEffect(detached)
	end
end

function Property:detach(type, effect)
	local detached = self.namedEffects:detach(type, effect)
	
	if detached	then
		self:_subEffect(detached)
	end
end

function Property:allAttachedStates()
	return pairs(self.namedEffects)
end

return Property
