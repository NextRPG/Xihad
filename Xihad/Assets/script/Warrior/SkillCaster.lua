local Table = require 'std.Table'
local SkillCaster = {
	skillRestCount = nil,
	skillInitCount = nil,
}
SkillCaster.__index = SkillCaster

function SkillCaster.new()
	return setmetatable({
			skillRestCount = {},	-- skill -> rest
			skillInitCount = {},	-- skill -> init
		}, SkillCaster)
end

local function isCastable(skill, time)
	return time >= 1
end

function SkillCaster:castableSkills()
	return Table.filteredPairs(self.skillRestCount, isCastable)
end

function SkillCaster:allSkills()
	return pairs(self.skillRestCount)
end

function SkillCaster:hasDrained(skill)
	return self:getRestCount(skill) < 1
end

function SkillCaster:canCast(skill, location)
	if self:hasDrained(skill) then
		return false
	end
	
	local launchables = skill:getLaunchableTiles(g_chessboard, 
		self:findPeer(c'Warrior'), location)
	
	return next(launchables) ~= nil
end

function SkillCaster:getRestCount(skill)
	return self.skillRestCount[skill] or 0
end

--- 
-- @return BattleResult
function SkillCaster:castSkill(skill, targetLocation, chessboard)
	local restCount = self.skillRestCount[skill]
	assert(restCount and restCount > 0, 
		string.format("Can't cast the specified skill: %s", skill:getName()))
	
	self.skillRestCount[skill] = restCount - 1
	
	return skill:resolve(self:findPeer(c'Warrior'), targetLocation, chessboard)
end

function SkillCaster:recover()
	local init, rest = self.skillInitCount, self.skillRestCount
	for skill, _ in pairs(self.skillRestCount) do
		rest[skill] = init[skill]
	end
end

function SkillCaster:learnSkill(skill, initialCount)
	assert(type(initialCount) == 'number' and initialCount > 0)
	if not self.skillInitCount[skill] then
		self.skillRestCount[skill] = initialCount
	else
		local delta = initialCount - self.skillInitCount[skill]
		if delta > 0 then
			local prev = self.skillRestCount[skill]
			local curr = math.max(initialCount, prev + delta)
			self.skillRestCount[skill] = curr
		end
	end
	
	self.skillInitCount[skill] = initialCount
end

function SkillCaster:forgetSkill(skill)
	self.skillRestCount[skill] = nil
	self.skillInitCount[skill] = nil
end

function SkillCaster:_getLocation(startLoc)
	return startLoc or self:findPeer(c'Warrior'):getLocation()
end

function SkillCaster:getCastableTiles(startLoc)
	startLoc = self:_getLocation(startLoc)
	
	local tiles = {}
	for skill, _ in self:castableSkills() do
		skill:getAllImpactTiles(g_chessboard, startLoc, tiles)
	end
	
	return Table.extractKeys(tiles)
end

return SkillCaster
