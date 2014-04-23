local Equation = require 'Warrior.Equation'

-- Battle Module 
local PropertyHost = {
	---
	-- Basic states that should be serialized on disk
	basicStates = nil,
	
	--- 
	-- Temporary states those generated in a battle
	attachedStates = nil,
	
	---
	-- Equations
	properties = nil,
}
PropertyHost.__index = PropertyHost

function PropertyHost.new()
	local o = setmetatable({
			basicStates = {},
			attachedStates = {},
			properties = {},
		}, PropertyHost)
	
	return o
end

function PropertyHost:get(name)
	local equation = self.properties[name]

	return equation and equation:calculate() or nil
end

function PropertyHost:addProperty(name)
	assert(not self.properties[name])
	self.basicStates[name] = 0
	self.properties[name] = Equation.new()
end

function PropertyHost:setBasic(name, basic)
	assert(self.properties[name])
	local prev = self.basicStates[name]
	local delta = basic - prev
	
	self.properties[name]:incOffset(delta)
	self.basicStates[name] = basic
end

function PropertyHost:getBasic(name)
	return self.basicStates[name]
end

function PropertyHost:attach(type, effect)
	self:detach(type)
	
	local prev = self.properties[effect.field]
	self.properties[effect.field] = prev + effect.equation
	self.attachedStates = effect
end

function PropertyHost:detach(type)
	local effect = attachedStates[type]
	if effect then
		local prev = self.properties[effect.field]
		self.properties[effect.field] = prev - effect.equation
		self.attachedStates[type] = nil
	end
end

return PropertyHost
