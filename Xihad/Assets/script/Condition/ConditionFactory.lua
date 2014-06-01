local BaseCondition = require 'Condition.BaseCondition'
local ConditionFactory = {}

-- TODO refactor
-- Move
-- function ConditionFactory.transferWarriorsWithTag(tag, isValidLoc)
-- 	local cond = BaseCondition.new()
-- 	function cond:updateCondition()
-- 		for object in g_scene:objectsWithTag(tag) do
-- 			local warrior = object:findComponent(c'Warrior')
-- 			if warrior and isValidLoc(warrior:getLocation()) then
-- 				self:setSatisfied()
-- 				break
-- 			end
-- 		end
-- 	end
	
-- 	return cond
-- end

-- function ConditionFactory.transferAllWarriorsWithTag(tag, isValidLoc)
-- 	local cond = BaseCondition.new()
-- 	function cond:updateCondition()
-- 		for object in g_scene:objectsWithTag(tag) do
-- 			local warrior = object:findComponent(c'Warrior')
-- 			if warrior and not isValidLoc(warrior:getLocation()) then
-- 				return
-- 			end
-- 		end
		
-- 		self:setSatisfied()
-- 	end
	
-- 	return cond
-- end

-- Dead
function ConditionFactory.beatWarriorsWithTag(tag)
	local cond = BaseCondition.new()
	function cond:updateCondition()
		for object in g_scene:objectsWithTag(tag) do
			local warrior = object:findComponent(c'Warrior')
			if warrior and not warrior:isDead() then
				return
			end
		end
		
		self:setSatisfied()
	end
	
	return cond
end

-- Capture
function ConditionFactory.captureWarriorsWithTag(tag)
	
end

-- Round
function ConditionFactory.roundExceed(roundCount)
	
end

function ConditionFactory.allOf(...)
	local andCondition = BaseCondition.new()
	local conditionArray = { ... }
	function andCondition:updateCondition()
		for _, cond in ipairs(conditionArray) do
			cond:updateCondition()
		end
	end
	
	function andCondition:onConditionSatisfied()
		for _, cond in ipairs(conditionArray) do
			if not cond:isSatisfied() then
				return
			end
		end
		
		self:setSatisfied()
	end
	
	for _, cond in ipairs(conditionArray) do
		cond:setSatisfiedListener(andCondition)
	end
	
	return andCondition
end

return ConditionFactory
