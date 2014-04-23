local Algorithm = require 'std.Algorithm'
local PropertyHost = require 'Warrior.PropertyHost'

local Ability = {
	-- Temporary
	active = false,
	roundListeners = nil,
	
	-- Temporary and persistant
	properties = nil,	-- Property Host
	
	-- Persistant
	career = nil,
	level  = 1,
	skills = nil,
	currentExp = 0,
	equipments = nil,
	
	sAllProperties = {}
}
Ability.__index = Ability

function Ability.new( data, object )
	assert(data.career)
	local o = setmetatable({
		roundListeners = {},
		properties = PropertyHost.new(),
		career = data.career,
		level  = data.level,
		skills = {},				-- skill -> time
		currentExp = data.currentExp,
		equipments = {},	-- part -> equipment
	}, Ability)
	
	for _, propertyName in ipairs(Ability.sAllProperties) do
		local value = data.properties[propertyName]
		if type(value) ~= 'number' then
			error('Missing property field in Ability data: '..propertyName)
		end
		
		o.properties:addProperty(propertyName)
		o.properties:setBasic(propertyName, value)
	end
	
	return o
end

function Ability.registerProperty(propertyName)
	assert(type(propertyName) == 'string')
	
	if #propertyName == 0 then
		error('Empty property name')
	end
	
	local methodName = 'get'..propertyName
	if Ability[methodName] ~= nil then
		error('Duplicated property name: ', propertyName)
	end
	
	Ability[methodName] = function (self)
		return self.properties:get(propertyName)
	end
	
	table.insert(Ability.sAllProperties, propertyName)
end

function Ability:canCastSkill(skill)
	return self.skills[skill] and self[skills] > 0
end

--- 
-- @return BattleResult
function Ability:castSkill(skill, target)
	local restCount = self.skills[skill]
	assert(restCount and restCount > 0, "Can't cast the specified skill")
	self.skills[skill] = restCount - 1
	
	return skill:cast(self, target)
end

function Ability:learnSkill(skill, initialCount)
	assert(type(initialCount) == 'number' and initialCount > 0)
	self.skills[skill] = initialCount
end

function Ability:forgetSkill(skill)
	self.skills[skill] = nil
end

function Ability:equip( equipment )
	local etype = equipment:getType()
	
	self:unequip(etype)
	assert(self.equipments[etype] == nil)
	
	local result = equipment:promote(self)
	self.equipments[etype] = equipment
end

function Ability:unequip( etype )
	local equipment = self.equipments[etype]
	
	if equipment then
		equipment:cancel(self)
		self.equipments[etype] = nil
	end
end

function Ability:getLevel()
	return self.level
end

function Ability:getCareer()
	return self.career
end

function Ability:addRoundListener(lis)
	assert(lis.onRoundBegin and lis.onRoundEnd)
	self.roundListeners[lis] = true
end

function Ability:removeRoundListener(lis)
	self.roundListeners[lis] = nil
end

function Ability:isActive()
	return self.active
end

-- Invoke when round begin
function Ability:activate()
	assert(not self.active)
	self.active = true
	
	for listener,_ in pairs(self.roundListeners) do
		listener:onRoundBegin(self)
	end
end

-- Invoke when round over
function Ability:deactivate()
	assert(self.active)
	self.active = false
	
	for listener,_ in pairs(self.roundListeners) do
		listener:onRoundEnd(self)
	end
end

function Ability:getNextLevelExp()
	return self.career:getNextLevelExp(self.level)
end

---
-- @return level-up information
function Ability:obtainExp(exp)
	self.currentExp = self.currentExp + exp
	local nextLevelExp = self:getNextLevelExp()
	if self.currentExp >= nextLevelExp then
		self.currentExp = self.currentExp - nextLevelExp
		self.level = self.level + 1
		
		return self.career:promote(self)
	end
end

function Ability:isDead()
	return self:getProperpty('currentHP') <= 0
end

---
-- BattleReuslt = { deltaHP, states }
function Ability:applyResult(battleResult)
	if self:isDead() then return end
	
	local deltaHP = battleResult.deltaHP or 0
	local currHP = Algorithm.clamp(self:getCurrentHP()+deltaHP, 0, self:getMaxHP())
	if deltaHP > 0 then
		-- TODO self.object:emittMessage()
	elseif deltaHP < 0 then
		-- TODO self.object:emittMessage()
		
		if self:isDead() then
			-- TODO
			-- self.object:emittMessage('Ability.dead', self)
			
			return
		end
	end
	
	if battleResult.states then
		-- TODO append states to property host
	end
end

return Ability
