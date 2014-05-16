local Algorithm = require 'std.Algorithm'
local PropertyHost = require 'Warrior.PropertyHost'

local Warrior = {
	team   = nil,
	career = nil,
	active = true,
	
	hitPoint   = 0,
	properties = nil,	-- Property Host
	
	roundListeners = nil,
	propertyListeners = nil,
	
	sAllProperties = {}
}
Warrior.__index = Warrior

function Warrior.new( data, object )
	assert(data.career)
	local o = setmetatable({
		team = data.team,
		career = data.career,
		properties = PropertyHost.new(),
		
		roundListeners = {},
		propertyListeners = {},
	}, Warrior)
	
	for _, pname in ipairs(Warrior.sAllProperties) do
		local value = data.properties[pname]
		if type(value) ~= 'number' then
			error('Missing property field in Warrior data: '..pname)
		end
		
		o:_initProperty(pname)
		o.properties:addProperty(pname)
		o.properties:setBasic(pname, value)
	end
	
	o:_initProperty('HitPoint')
	o:_initProperty('Dead')
	o:_setHitPoint(o:getMHP())
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
		return self.properties:get(pname)
	end
	
	table.insert(Warrior.sAllProperties, pname)
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
	-- return self.career:
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
