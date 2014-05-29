local base = require 'Tactic.Grader'
local Table= require 'std.Table'
local CastGrader = setmetatable({}, base)

local function averageDamage(result)
	return (result:getMaxDamage() + result:getMinDamage()) * 0.5
end

function CastGrader.maxTotalDamage()
	return function (warrior, enemy, skill, impactCenter)
		local results = skill:resolve(warrior, impactCenter, g_chessboard)
		local damage = 0
		for barrier, result in pairs(results) do
			if barrier:findPeer(c'Warrior') then
				damage = damage + averageDamage(result)
			end
		end
		
		return damage
	end
end

function CastGrader.maxDamage()
	return function (warrior, enemy, skill, impactCenter)
		local results = skill:resolve(warrior, impactCenter, g_chessboard)
		local result = results[enemy:findPeer(c'Barrier')]
		if not result or not result:hasDamage() then
			return -1 
		else
			return averageDamage(result)
		end
	end
end

function CastGrader.mostInjured()
	return function (warrior, enemy, skill, impactCenter)
		local results = skill:resolve(warrior, impactCenter, g_chessboard)
		return Table.count(results)
	end
end

return CastGrader
