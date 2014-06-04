local base = require 'route.BaseImpactRange'
local Location = require 'route.Location'
local ProjectiveImpactRange = setmetatable({}, base)
ProjectiveImpactRange.__index = ProjectiveImpactRange

function ProjectiveImpactRange.new(relativeLocationSet)
	return setmetatable(base.new(relativeLocationSet), ProjectiveImpactRange)
end

---
-- Override for performance
function ProjectiveImpactRange:canImpact(launcherCenter, impactCenter, target)
	target = impactCenter and target - impactCenter or target
	return self.relativeLocationSet:contains(target)
end

function ProjectiveImpactRange:_createTransform(launcherCenter, impactCenter)
	return Location.copy
end

if select('#', ...) == 0 then 
	local OrderedSet = require 'std.OrderedSet'
	local set = OrderedSet.new()
	set:insert(Location.new( 0, 0))
	set:insert(Location.new( 0, 1))
	set:insert(Location.new( 0,-1))
	set:insert(Location.new( 1, 0))
	set:insert(Location.new(-1, 0))
	
	local range = ProjectiveImpactRange.new(set)
	
	assert(range:canImpact(nil, nil, Location.new( 1,  0)))
	assert(range:canImpact(nil, nil, Location.new( 0,  1)))
	assert(range:canImpact(nil, nil, Location.new( 0,  0)))
	assert(range:canImpact(nil, Location.new( 0,  0), Location.new(1, 0)))
	assert(not range:canImpact(nil, Location.new( 0,  0), Location.new(2, 0)))
	assert(not range:canImpact(nil, nil, Location.new( 1,  1)))
	
	local impactCount = 0
	local launchLoc = Location.new(1, 2)
	range:traverse(Location.new(), launchLoc, function (loc)
		assert(range:canImpact(Location.new(), launchLoc, loc))
		impactCount = impactCount+1
	end)
	assert(impactCount == 5)
end

return ProjectiveImpactRange
