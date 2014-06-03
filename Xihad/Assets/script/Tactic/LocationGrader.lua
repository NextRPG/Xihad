local base = require 'Tactic.Grader'
local LocationGrader = setmetatable({}, base)

function LocationGrader.stationary()
	return function (warrior, enemy, cast, location)
		if location == warrior:getLocation() then
			return 1, true
		else
			return 0
		end
	end
end

function LocationGrader.farAwayFromEnemy()
	return function (warrior, enemy, cast, location)
		return location:distance(enemy:getLocation())
	end
end

return LocationGrader
