local Class = require 'std.Class'
local ProbabilityValue = require 'Warrior.ProbabilityValue'
local WarriorHitResult = {
	sourceWarrior = nil,
	targetWarrior = nil,
	
	repelDistance = 0,
	recoveries = nil,
	damages = nil,
	states  = nil,
}
WarriorHitResult.__index = WarriorHitResult

function WarriorHitResult.new(source, target)
	return setmetatable({
			sourceWarrior = source,
			targetWarrior = target,
			recoveries = ProbabilityValue.new(),
			damages = ProbabilityValue.new(),
			states  = {},
		}, WarriorHitResult)
end

function WarriorHitResult:getTargetBarrier()
	return self.targetWarrior:findPeer(c'Barrier')
end

Class.delegate(WarriorHitResult, 'hasDamage',	 'damages', 'empty')
Class.delegate(WarriorHitResult, 'addDamage', 	 'damages', 'addValue')
Class.delegate(WarriorHitResult, 'getMinDamage', 'damages', 'getMin')
Class.delegate(WarriorHitResult, 'getMaxDamage', 'damages', 'getMax')
Class.delegate(WarriorHitResult, 'randomDamage', 'damages', 'random')

Class.delegate(WarriorHitResult, 'hasRecovery',	   'recoveries', 'empty')
Class.delegate(WarriorHitResult, 'addRecovery',	   'recoveries', 'addValue')
Class.delegate(WarriorHitResult, 'getMinRecovery', 'recoveries', 'getMin')
Class.delegate(WarriorHitResult, 'getMaxRecovery', 'recoveries', 'getMax')
Class.delegate(WarriorHitResult, 'randomRecovery', 'recoveries', 'random')

function WarriorHitResult:_getDamage()
	local skillDamage = self:randomDamage()
	local atk = self.sourceWarrior:getATK()
	local dfs = self.targetWarrior:getDFS()
	return math.abs(atk + skillDamage - dfs)
end

function WarriorHitResult:hasBuff()
	return next(self.states) ~= nil
end

function WarriorHitResult:canRepel()
	return self.repelDistance ~= 0
end

function WarriorHitResult:_applyDamage()
	local damage
	if math.random() >= self:getHitRate() then
		-- miss
		print('damage miss')
		damage = 1
	elseif math.random() < self:getKillRate() then
		-- TODO how to present kill
		-- self.sourceWarrior:launchKill()
		print('damage kill')
		damage = self.targetWarrior:getHitPoint()
	else-- normal
		print('damage normal')
		damage = self:_getDamage()
	end
	
	self.targetWarrior:takeDamage(damage)
end

function WarriorHitResult:_applyRecovery()
	self.targetWarrior:takeRecovery(self:randomRecovery())
end

function WarriorHitResult:_applyBuff()
	for buff, probability in pairs(self.states) do
		if math.random() < probability then
			
		end
	end
end

---
-- 1. play hp drop animation
-- 2. add buff effect
-- 3. repel
-- 4. play dead or idle animation
function WarriorHitResult:apply()
	if self.repelDistance ~= 0 then
		
	end
	
	if self:hasDamage() then
		self:_applyDamage()
	end
	
	if self:hasRecovery() then
		self:_applyRecovery()
	end
	
	if self:hasBuff() then
		self:_applyBuff()
	end
end

function WarriorHitResult:getHitRate()
	return 1
end

function WarriorHitResult:getKillRate()
	return 0
end

function WarriorHitResult:isValid()
	return self:hasDamage() 
		or self:hasRecovery()
		or self:canRepel() 
		or self:hasBuff()
end

return WarriorHitResult
