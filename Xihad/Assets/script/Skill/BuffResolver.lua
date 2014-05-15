--- TODO
-- local base = require 'Skill.ToWarriorResolver'
-- local BuffResolver = {
-- 	centerOnly = true,
-- 	repelDistance = 1,
-- }
-- BuffResolver.__index = BuffResolver
-- setmetatable(BuffResolver, base)

-- function BuffResolver.new(centerOnly, distance)
-- 	local obj = setmetatable(base.new(), BuffResolver)
-- 	obj.centerOnly = centerOnly
-- 	obj.repelDistance = math.floor(distance)
-- 	return obj
-- end

-- function BuffResolver:_resolve(sourceWarrior, targetWarrior, relativeLoc, result)
-- 	if relativeLoc ~= Location.new() and not self.centerOnly then
-- 		return 
-- 	end
	
-- 	result.repelDst = self.repelDistance
-- end

-- return BuffResolver
