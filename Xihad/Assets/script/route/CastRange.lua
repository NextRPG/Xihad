local Location = require 'route.Location'
local CastRange = {
	minLaunchDistance = 1,
	maxLaunchDistance = 1,
}
CastRange.__index = CastRange

function CastRange.new(minD, maxD)
	local r = {}
	if type(minD) == 'number' then
		minD = math.max(0, minD)
	end
	
	return setmetatable({
			minLaunchDistance = minD,
			maxLaunchDistance = math.max(minD or 1, maxD or 1),
		}, CastRange)
end

function CastRange:canLaunch(target, launcherCenter)
	launcherCenter = launcherCenter or Location.new(0, 0)
	local d = target:distance(launcherCenter)
	return self.minLaunchDistance <= d and d <= self.maxLaunchDistance
end

function CastRange:traverse(launcherCenter, f)
	launcherCenter = launcherCenter or Location.new(0, 0)
	
	local offset = Location.new()
	for dist = self.minLaunchDistance, self.maxLaunchDistance do
		local xWeight
		for xWeight = -dist, dist do
			offset.x = xWeight
			offset.y = dist - math.abs(offset.x)
			if f(launcherCenter + offset) then return end
			
			if offset.y ~= 0 then
				offset.y = -offset.y
				if f(launcherCenter + offset) then return end
			end
		end
	end
end

if select('#', ...) == 0 then 
	local range = CastRange.new(2, 4)
	assert(range:canLaunch(Location.new(2, 0)))
	assert(range:canLaunch(Location.new(0, 1), Location.new(2, 1)))
	assert(range:canLaunch(Location.new(3, 0)))
	assert(range:canLaunch(Location.new(0, 4)))
	assert(not range:canLaunch(Location.new(1, 4)))
	assert(not range:canLaunch(Location.new(1, 0)))
	
	local launchableCount = 0
	local launcherLoc = Location.new(1, 1)
	range:traverse(launcherLoc, function(loc)
		assert(range:canLaunch(loc, launcherLoc))
		launchableCount = launchableCount+1
	end)
	assert(launchableCount==36)
end

return CastRange
