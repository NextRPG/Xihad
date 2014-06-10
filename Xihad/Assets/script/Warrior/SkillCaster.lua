local Table = require 'std.Table'
local Algorithm = require 'std.Algorithm'
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

function SkillCaster:hasDrained(skill)
	if not self:hasLearnedSkill(skill) then
		error('Non learned skill')
	end
	
	return self:getRestCount(skill) < 1
end

function SkillCaster:hasLearnedSkill(skill)
	return self.skillInitCount[skill] ~= nil
end

function SkillCaster:allSkills()
	local function iter(self, prevKey)
		local skill, init = next(self.skillInitCount, prevKey)
		return skill, self.skillRestCount[skill], init
	end
	
	return iter, self
end

function SkillCaster:getRestCount(skill)
	return self.skillRestCount[skill]
end

function SkillCaster:getInitCount(skill)
	return self.skillInitCount[skill]
end

function SkillCaster:canCast(skill, location)
	if self:hasDrained(skill) then
		return false
	end
	
	local launchables = skill:getLaunchableTiles(g_chessboard, 
		self:findPeer(c'Warrior'), location)
	
	return next(launchables) ~= nil
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

function SkillCaster:setRest(skill, newValue)
	assert(self:hasLearnedSkill(skill))
	
	local max = self.skillInitCount[skill]
	self.skillRestCount[skill] = Algorithm.clamp(newValue, 0, max)
end

function SkillCaster:recover(percentage)
	local init, rest = self.skillInitCount, self.skillRestCount
	for skill, rest in pairs(self.skillRestCount) do
		local toAdd = self.skillInitCount[skill] * percentage
		self:setRest(skill, math.floor(rest + toAdd))
	end
end

function SkillCaster:learnSkill(skill, initialCount)
	assert(type(initialCount) == 'number' and initialCount > 0)
	local prevInit = self.skillInitCount[skill] or 0
	local delta = initialCount - prevInit
	if delta > 0 then
		local prevRest = self.skillRestCount[skill] or 0
		self.skillInitCount[skill] = prevInit + delta
		self.skillRestCount[skill] = prevRest + delta
	end
end

function SkillCaster:forgetSkill(skill)
	self.skillRestCount[skill] = nil
	self.skillInitCount[skill] = nil
end

function SkillCaster:_getLocation(startLoc)
	return startLoc or self:findPeer(c'Warrior'):getLocation()
end

function SkillCaster:getCastableTiles(startLoc, set)
	startLoc = self:_getLocation(startLoc)
	set = set or {}
	
	for skill, _ in self:castableSkills() do
		skill:getAllImpactTiles(g_chessboard, startLoc, set)
	end
	
	return set
end

return SkillCaster
