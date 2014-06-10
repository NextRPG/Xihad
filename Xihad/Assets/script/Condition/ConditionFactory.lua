local functional = require 'std.functional'
local BaseCondition = require 'Condition.BaseCondition'
local ConditionFactory = {}

function ConditionFactory.tagCondition(tag, algo, predicate)
	local cond = BaseCondition.new()
	function cond:_updateCondition()
		local iter = functional.bind2(g_scene.objectsWithTag, g_scene, tag)
		return algo(iter, predicate)
	end
	
	return cond
end

function ConditionFactory.transferWarrior(tag, algo, isValidLoc)
	local function predicate(object)
		return isValidLoc(object:findComponent(c'Warrior'):getLocation())
	end
	
	return ConditionFactory.tagCondition(tag, algo, predicate)
end

function ConditionFactory.beatWarrior(tag, algo)
	local function predicate(object)
		return object:findComponent(c'Warrior'):isDead()
	end
	
	return ConditionFactory.tagCondition(tag, algo, predicate)
end

-- Capture
function ConditionFactory.captureWarrior(tag)
	
end

-- Round
function ConditionFactory.roundExceed(roundCount)
	local cond = BaseCondition.new()
	function cond:_updateCondition(context)
		return context:getCurrentRound() > roundCount
	end
	
	return cond
end

function ConditionFactory.allOf(...)
	local andCondition = BaseCondition.new()
	local conditionArray = { ... }
	function andCondition:_updateCondition()
		local satisfied = true
		for _, cond in ipairs(conditionArray) do
			cond:_updateCondition()
			satisfied = cond:isSatisfied() and satisfied
		end
		
		return satisfied
	end
	
	for _, cond in ipairs(conditionArray) do
		cond:setSatisfiedListener(andCondition)
	end
	
	return andCondition
end

return ConditionFactory
