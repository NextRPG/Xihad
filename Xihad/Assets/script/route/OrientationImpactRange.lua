local base = require 'route.BaseImpactRange'
local Coordinate2D = require 'route.Coordinate2D'
local OrientationImpactRange = setmetatable({}, base)
OrientationImpactRange.__index = OrientationImpactRange

function OrientationImpactRange.new(relativeLocationSet)
	return setmetatable(base.new(relativeLocationSet), OrientationImpactRange)
end

function OrientationImpactRange:_createTransform(launcherCenter, impactCenter)
	local coord = Coordinate2D.build8Directions(impactCenter - launcherCenter)
	return function (impactLoc)
		return coord:transform(impactLoc)
	end
end

if select('#', ...) == 0 then 
	local Location = require 'route.Location'
	local OrderedSet = require 'std.OrderedSet'
	local set = OrderedSet.new()
	set:insert(Location.new( 1, 2))
	local range = OrientationImpactRange.new(set)
	
	local standLoc = Location.new(3, 6)
	local launchLoc= Location.new(4, 6)
	assert(range:canImpact(standLoc, launchLoc, Location.new(6, 5)))
	assert(not range:canImpact(standLoc, launchLoc, Location.new(5, 8)))
	
	local standLoc = Location.new(4, 5)
	local launchLoc= Location.new(4, 6)
	assert(range:canImpact(standLoc, launchLoc, Location.new(5, 8)))
	
	local standLoc = Location.new(4, 5)
	local launchLoc= Location.new(3, 4)
	assert(range:canImpact(standLoc, launchLoc, Location.new(0, 3)))
end

return OrientationImpactRange