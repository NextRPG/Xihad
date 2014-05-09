local WarriorHitResult = {
	sourceWarrior = nil,
	targetWarrior = nil,
	damage 	= -1,
	recovery= -1,
	repelDst= 0,
	states  = nil,
}
WarriorHitResult.__index = WarriorHitResult

function WarriorHitResult.new(source, target)
	return setmetatable({
			sourceWarrior = source,
			targetWarrior = target,
			states = {},
		}, WarriorHitResult)
end

function WarriorHitResult:getTargetBarrier()
	return self.targetWarrior
end

function WarriorHitResult:apply()
	print(self.damage)
	-- if math.random() > self:getHitRate() then
	-- 	return self:_handleMiss()
	-- elseif math.random() < self:getKillRate() then
	-- 	return self:_handleKill()
	-- else
	-- 	return self:_handleNormal()
	-- end
end

function WarriorHitResult:getHitRate()
	
end

function WarriorHitResult:getKillRate()
	
end

function WarriorHitResult:isValid()
	return self.damage >= 0 or self.recovery >= 0 or 
			self.repelDst > 0 or #self.states > 0
end

return WarriorHitResult
