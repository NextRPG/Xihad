local Level = {
	exp 	= 0,
	level 	= 1,
	grader 	= nil,
}
Level.__index = Level

function Level.new( level, grader, object )
	assert(grader ~= nil)
	local obj = setmetatable({
			level  = level,
			grader = grader,
		}, Level)
	return obj
end

function Level:getLevel()
	return self.level
end

function Level:getNextLevelExp()
	return self.grader:getNextLevelExp(self.level)
end

---
-- @return level-up information
function Level:_obtainExp(exp)
	self.exp = self.exp + exp
	local object = self:getHostObject()
	
	while true do
		local nextLevelExp = self:getNextLevelExp()
		if self.exp >= nextLevelExp then
			self.exp = self.exp - nextLevelExp
			self.level = self.level + 1
			
			coroutine.yield(self.grader:promote(object, self.level))
		else
			break
		end
	end
end

function Level:obtainExp(exp)
	return coroutine.wrap(function()
			return self:_obtainExp(exp)
		end)
end

if select('#', ...) == 0 then 
	local grader = {
		promote = function (self, obj, level)
			return level
		end,
		
		getNextLevelExp = function (self, level)
			return level*2
		end,
	}
	
	local levelComp = Level.new(10, grader, nil)
	levelComp.getHostObject = function () return nil end
	
	assert(levelComp:getNextLevelExp() == 20)
	local continueLevelUp = levelComp:obtainExp(100)
	
	local level = levelComp:getLevel()
	local levelUpResult
	repeat
		levelUpResult = continueLevelUp()
		
		assert(levelUpResult and levelComp:getLevel() == level+1 
							 or  levelComp:getLevel() == level)
		
		level = levelComp:getLevel()
	until not levelUpResult
end

return Level