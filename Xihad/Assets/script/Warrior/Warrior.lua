local Array = require 'std.Array'
local functional= require 'std.functional'
local Algorithm = require 'std.Algorithm'
local Property  = require 'Warrior.Property'
local NamedEffects = require 'Warrior.NamedEffects'

if select('#', ...) ~= 0 then 
	assert(select(1, ...) == 'Warrior', 'require for "Warrior" instead')
end

local Warrior = {
	team   = nil,
	career = nil,
	active = true,
	
	hitPoint   = 0,
	properties = nil,	-- Property Host
	
	namedEffects = nil,	-- To avoid duplicated effect to a warrior
	roundListeners = nil,
	propertyListeners = nil,
	
	sAllProperties = {},
}
Warrior.__index = Warrior

function Warrior.new( data, object )
	assert(data.career)
	local o = setmetatable({
		team = data.team,
		career = data.career,
		properties = {},
		
		namedEffects = NamedEffects.new(),
		roundListeners = {},
		propertyListeners = {},
	}, Warrior)
	
	for _, pname in ipairs(Warrior.sAllProperties) do
		local value = data.properties[pname]
		if type(value) ~= 'number' then
			error('Missing property field in Warrior data: '..pname)
		end
		
		o:_initProperty(pname)
		local newProperty = Property.new()
		o.properties[pname] = newProperty
		newProperty:addBasic(value)
	end
	
	o:_initProperty('HitPoint')
	o:_initProperty('Dead')
	o:_setHitPoint(o:getMHP())	-- TODO REFACTOR
	return o
end

function Warrior:_initProperty(pname)
	self.propertyListeners[pname] = {}
end

function Warrior.registerProperty(pname)
	assert(type(pname) == 'string')
	
	if #pname == 0 then
		error('Empty property name')
	end
	
	local methodName = 'get'..pname
	if Warrior[methodName] ~= nil then
		error('Duplicated property name: ', pname)
	end
	
	Warrior[methodName] = function (self)
		return self:get(pname)
	end
	
	table.insert(Warrior.sAllProperties, pname)
end

function Warrior:exchangables(startLoc)
	startLoc = startLoc or self:getLocation()
	local locations = {
		startLoc:left(),
		startLoc:right(),
		startLoc:top(),
		startLoc:down()
	}
	
	local iter = functional.bind1(Array.elements, locations)
	local filter = function (tile)
		local adjWarrior = tile:getWarrior()
		if adjWarrior and adjWarrior:isLeagueWith(self) then
			local adjHasItem = adjWarrior:findPeer(c'Parcel'):hasItem()
			local selfHasItem= self:findPeer(c'Parcel'):hasItem()
			
			return adjHasItem or selfHasItem
		end
		
		return false
	end
	
	return iter, filter
end

function Warrior:getTeam()
	return self.team
end

function Warrior:isLeagueWith(other)
	return self.team == other.team
end

function Warrior:isEnemyWith(other)
	return self.team ~= other.team
end

function Warrior:getLocation()
	return self:findPeer(c'Barrier'):getTile():getLocation()
end

function Warrior:getActionPoint()
	return self:getMAP()
end

function Warrior:getCareer()
	return self.career
end

function Warrior:getNature()
	return 'unknown'
end

function Warrior:isActive()
	return self.active
end

-- Invoke when round begin
function Warrior:activate()
	assert(not self.active)
	self.active = true
	
	for listener, _ in pairs(self.roundListeners) do
		listener:onRoundBegin(self)
	end
end

-- Invoke when round over
function Warrior:deactivate()
	assert(self.active)
	self.active = false
	
	for listener,_ in pairs(self.roundListeners) do
		listener:onRoundEnd(self)
	end
end

function Warrior:addRoundListener(lis)
	assert(lis.onRoundBegin and lis.onRoundEnd)
	self.roundListeners[lis] = true
end

function Warrior:removeRoundListener(lis)
	self.roundListeners[lis] = nil
end

function Warrior:_getListeners(pname)
	local listeners = self.propertyListeners[pname]
	assert(listeners, string.format('No such property: %s', pname))
	return listeners
end

function Warrior:_checkEffectBinding(effect)
	assert(effect:isBound() and effect:getBinding() == self, 
		'Invoke effect:bind() instead')
end

function Warrior:_checkEffectUnbinding(effect)
	assert(not effect:isBound() and effect:getBinding() == self, 
		'Invoke effect:unbind() instead')
end

function Warrior:registerEffect(type, effect)
	self:_checkEffectBinding(effect)
	self.namedEffects:attach(type, effect)
end

function Warrior:unregisterEffect(type, effect)
	self:_checkEffectUnbinding(effect)
	self.namedEffects:detach(type, effect)
end

function Warrior:addPropertyListener(pname, lis)
	self:_getListeners(pname)[lis] = true
	
	-- initial update
	lis(self, pname, nil)
end

function Warrior:removePropertyListener(pname, lis)
	self:_getListeners(pname)[lis] = nil
end

function Warrior:_firePropertyChange(pname, prev)
	for lis, _ in pairs(self:_getListeners(pname)) do
		lis(self, pname, prev)
	end
end

function Warrior:_getProperty(pname)
	if not self.properties[pname] then
		error('no such property', pname)
	end
	
	return self.properties[pname]
end

function Warrior:get(pname)
 	return self:_getProperty(pname):get()
end

function Warrior:addProperty(pname, equation)
	local prev = self:get(pname)
	self:_getProperty(pname):addEquation(equation)
	self:_firePropertyChange(pname, prev)
end

function Warrior:getBasic(pname)
	return self:_getProperty(pname):getBasic()
end

function Warrior:addBasic(pname, delta)
	local prev = self:get(pname)
	self:_getProperty(pname):addBasic(delta)
	self:_firePropertyChange(pname, prev)
end

function Warrior:getHitPoint()
	return self.hitPoint
end

function Warrior:isDead()
	return self.hitPoint <= 0
end

function Warrior:_clampHitPoint(hp)
	return Algorithm.clamp(hp, 0, self:getMHP())
end

function Warrior:_setHitPoint(newHitPoint)
	local prev = self:getHitPoint()
	self.hitPoint = self:_clampHitPoint(newHitPoint)
	self:_firePropertyChange('HitPoint', prev)
end

function Warrior:takeDamage(damage)
	assert(damage >= 0)
	if self:isDead() then
		error('The warrior has already died')
	end
	
	self:_setHitPoint(self:getHitPoint() - damage)
	if self:isDead() then
		self:_firePropertyChange('Dead', true)
	end
end

return Warrior
