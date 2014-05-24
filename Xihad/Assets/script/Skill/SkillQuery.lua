local Table = require 'std.Table'
local SkillRegistry = require 'Skill.SkillRegistry'
local SkillQuery = {}

function SkillQuery:_byName(skillName)
	return SkillRegistry.findSkillByName(skillName)
end

function SkillQuery.getLaunchableTiles(warrior, skillName, array)
	array = array or false
	local skill = SkillRegistry.findSkillByName(skillName)
	
	local set = skill:getLaunchableTiles(g_chessboard, warrior)
	if array then
		return Table.extractKeys(set)
	else
		return set
	end
end

return SkillQuery