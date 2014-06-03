local base = require 'Tactic.Grader'
local WarriorQuery= require 'Warrior.WarriorQuery'
local EnemyGrader = setmetatable({}, base)

---
-- if the distance to enemy is greater than maxDistance, we don't choose it
function EnemyGrader.nearest(maxCost)
	maxCost = maxCost or 10000000
	return function (warrior, enemy)
		local cost = WarriorQuery.minCostToApproach(warrior, enemy)
		
		if not cost then
			return -1
		else
			local enough = (cost == 1)
			return maxCost - cost, enough
		end
	end
end

---
-- if the the enmey's hitpoint is greater than maxHitPoint, we don't choose it
function EnemyGrader.weakest(maxHitPoint)
	maxHitPoint = maxHitPoint or 10000000
	return function (warrior, enemy)
		return maxHitPoint - enemy:getHitPoint()
	end
end

return EnemyGrader
