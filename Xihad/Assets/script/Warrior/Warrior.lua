local Algorithm = require 'std.Algorithm'
local PropertyHost = require 'Warrior.PropertyHost'

local Warrior = {
	team = nil,
	active = true,
	roundListeners = nil,
	
	properties = nil,	-- Property Host
	hitPoint = nil,
	
	career = nil,
	
	sAllProperties = {}
}
Warrior.__index = Warrior

function Warrior.new( data, object )
	assert(data.career)
	local o = setmetatable({
		team = data.team,
		roundListeners = {},
		properties = PropertyHost.new(),
		career = data.career,
	}, Warrior)
	
	for _, propertyName in ipairs(Warrior.sAllProperties) do
		local value = data.properties[propertyName]
		if type(value) ~= 'number' then
			error('Missing property field in Warrior data: '..propertyName)
		end
		
		o.properties:addProperty(propertyName)
		o.properties:setBasic(propertyName, value)
	end
	
	return o
end

function Warrior.registerProperty(propertyName)
	assert(type(propertyName) == 'string')
	
	if #propertyName == 0 then
		error('Empty property name')
	end
	
	local methodName = 'get'..propertyName
	if Warrior[methodName] ~= nil then
		error('Duplicated property name: ', propertyName)
	end
	
	Warrior[methodName] = function (self)
		return self.properties:get(propertyName)
	end
	
	table.insert(Warrior.sAllProperties, propertyName)
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

function Warrior:addRoundListener(lis)
	assert(lis.onRoundBegin and lis.onRoundEnd)
	self.roundListeners[lis] = true
end

function Warrior:removeRoundListener(lis)
	self.roundListeners[lis] = nil
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

function Warrior:isDead()
	return self:getProperpty('currentHP') <= 0
end

---
-- BattleReuslt = { deltaHP, states }
function Warrior:takeDamage(damage)
	assert(damage >= 0)
	local currHP = Algorithm.clamp(self:getCurrentHP()+deltaHP, 0, self:getMaxHP())
	-- TODO self.object:emittMessage()
	
	if self:isDead() then
		-- TODO
		-- self.object:emittMessage('Warrior.dead', self)
		
		return
	end
end

return Warrior
