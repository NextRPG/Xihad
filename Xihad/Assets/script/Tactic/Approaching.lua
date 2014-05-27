local functional = require 'std.functional'
local WarriorQuery = require 'Warrior.WarriorQuery'
local GradeSelector= require 'Tactic.GradeSelector'
local Approaching = {}

function Approaching.fixedTarget(loc)
	loc = loc:copy()
	return function (warrior, bestEnemy)
		return loc
	end
end

function Approaching.bestEnemy()
	return function (warrior, bestEnemy)
		if bestEnemy then
			return bestEnemy:getLocation()
		end
	end
end

function Approaching.enemyGrader(grader, tag)
	return function (warrior, bestEnemy)
		local enemy = GradeSelector.select(
			WarriorQuery.withTag(tag),
			functional.bind1(grader, warrior))
		
		if enemy then
			return select(2, WarriorQuery.minCostToApproach(warrior, enemy))
		end
	end
end

return Approaching
