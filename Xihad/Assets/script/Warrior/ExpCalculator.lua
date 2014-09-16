local Class = require 'std.Class'
local ReadonlyWarrior = require 'Warrior.ReadonlyWarrior'
local ExpCalculator = {
	damageAccum   = 0,
	recoveryAccum = 0,
	dead = false,
	
	expAccum = 0,
}
ExpCalculator.__index = ExpCalculator

function ExpCalculator.new()
	return setmetatable({}, ExpCalculator)
end

function ExpCalculator:onResult(targetBarrier, resultType, resultParam)
	if resultType == 'damage' then
		self.damageAccum = self.damageAccum + resultParam
	elseif resultType == 'recovery' then
		self.recoveryAccum = self.recoveryAccum + resultParam
	elseif resultType == 'dead' then
		self.dead = resultParam
	end
end

function ExpCalculator:_clear_phase()
	self.damageAccum = nil
	self.recoveryAccum = nil
	self.dead = nil
end

function ExpCalculator:_add_exp(n)
	self.expAccum = self.expAccum + n
end

function ExpCalculator:complete(targetBarrier)
	local targetWarrior = targetBarrier:findPeer(c'Warrior')
	if targetWarrior ~= nil then
		self:_add_exp(self.damageAccum / 10)
		self:_add_exp(self.recoveryAccum / 10)
		
		if self.dead then
			self:_add_exp(10)
		end
	end
	
	self:_clear_phase()
end

function ExpCalculator:getResult()
	return self.expAccum
end

function ExpCalculator:clear()
	self:_clear_phase()
	self.expAccum = nil
end

return ExpCalculator