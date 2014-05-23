local Array = require 'std.Array'
local Table = require 'std.Table'
local SkillCaster = {
	skills = nil,
}
SkillCaster.__index = SkillCaster

function SkillCaster.new()
	return setmetatable({
			skills = {},	-- skill -> time
		}, SkillCaster)
end

function SkillCaster:castableSkills()
	return function(t, lastKey) 
		local skill, time
		
		repeat
			skill, time = next(t, lastKey)
		until not skill or time > 0
			
		return skill
	end, self.skills
end

function SkillCaster:allSkills()
	return pairs(self.skills)
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
	return self.skills[skill] or 0
end

--- 
-- @return BattleResult
function SkillCaster:castSkill(skill, targetLocation, chessboard)
	local restCount = self.skills[skill]
	assert(restCount and restCount > 0, 
		string.format("Can't cast the specified skill: %s", skill:getName()))
	
	self.skills[skill] = restCount - 1
	
	return skill:resolve(self:findPeer(c'Warrior'), targetLocation, chessboard)
end

function SkillCaster:learnSkill(skill, initialCount)
	assert(type(initialCount) == 'number' and initialCount > 0)
	self.skills[skill] = initialCount
end

function SkillCaster:forgetSkill(skill)
	self.skills[skill] = nil
end

function SkillCaster:_getLocation(startLoc)
	return startLoc or self:findPeer(c'Warrior'):getLocation()
end

function SkillCaster:getCastableTiles(startLoc)
	startLoc = self:_getLocation(startLoc)
	
	local tiles = {}
	for skill, restTimes in pairs(self.skills) do
		if restTimes > 0 then
			skill:getAllImpactTiles(g_chessboard, startLoc, tiles)
		end
	end
	
	return Array.keys(tiles)
end

return SkillCaster
