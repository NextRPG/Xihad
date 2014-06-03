local base = require 'GameFlow.BattleTeam'
local Iterator = require 'std.Iterator'
local functional = require 'std.functional'

local XihadBattleTeam = setmetatable({}, base)
XihadBattleTeam.__index = XihadBattleTeam

function XihadBattleTeam.new(teamName, controller)
	return setmetatable(base.new(teamName, controller), XihadBattleTeam)
end

function XihadBattleTeam:allWarriors()
	return Iterator.transform(
		function(object) 
			if object then
				return object:findComponent(c'Warrior')
			end
			return nil
		end,
		
		g_scene:objectsWithTag(self:getTeamName()))
end

-- TODO
function XihadBattleTeam:belongToTeam(warrior)
	return warrior:getTeam() == self:getTeamName()
end


return XihadBattleTeam