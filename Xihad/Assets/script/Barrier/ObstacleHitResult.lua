local ObstacleHitResult = {}
ObstacleHitResult.__index = ObstacleHitResult

function ObstacleHitResult.new()
	local obj = setmetatable({ }, ObstacleHitResult)
	return obj
end

---
-- Play hit animation?
function ObstacleHitResult:onHitBegin(sourceWarrior, targetBarrier)
end

---
-- Hit animation end
-- Take damages?
function ObstacleHitResult:apply(sourceWarrior, targetBarrier, lis)
end

---
-- Check death
function ObstacleHitResult:onHitEnd()
end

return ObstacleHitResult