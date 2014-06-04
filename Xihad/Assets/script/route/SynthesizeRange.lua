local Array = require 'std.Array'
local SynthesizeRange = {
	castRange = nil,
	impactRange = nil,
}
SynthesizeRange.__index = SynthesizeRange

function SynthesizeRange.new(castRange, impactRange)
	return setmetatable({
			castRange = castRange,
			impactRange = impactRange,
		}, SynthesizeRange)
end

function SynthesizeRange:traverseLaunchableLocations(launcherCenter, f)
	self.castRange:traverse(launcherCenter, f)
end

function SynthesizeRange:traverseImpactLocations(launcherCenter, impactCenter, f)
	self.impactRange:traverse(launcherCenter, impactCenter, f)
end

function SynthesizeRange:traverseAllImpactLocations(launcherCenter, impactCenterFilter, f)
	local arr = {}
	
	self:traverseLaunchableLocations(launcherCenter, function(impactCenter)
		if impactCenterFilter and not impactCenterFilter(impactCenter) then
			return	-- it didn't pass the filter
		end
		
		self:traverseImpactLocations(launcherCenter, impactCenter, 
			function(impactLocation)
				local idx = Array.bsearch(arr, impactLocation)
				if idx < 0 then
					Array.insert(arr, -idx, impactLocation)	-- bad performance
					return f(impactLocation:copy())
				end
			end)
	end)
end

if select('#', ...) == 0 then 
	local Location = require 'route.Location'
	local CastRange= require 'route.CastRange'
	local OrderedSet = require 'std.OrderedSet'
	local ImpactRange= require 'route.ProjectiveImpactRange'
	
	local castRange = CastRange.new(2, 4)
	local set = OrderedSet.new()
	set:insert(Location.new( 0, 0))
	set:insert(Location.new( 0, 1))
	set:insert(Location.new( 0,-1))
	set:insert(Location.new( 1, 0))
	set:insert(Location.new(-1, 0))
	local impactRange = ImpactRange.new(set)
	local range = SynthesizeRange.new(castRange, impactRange)
	
	-- 10 + (9+7+5+3+1)*2 == 60
	local allImpact = 0
	range:traverseAllImpactLocations(Location.new(6, 6), nil, function(x, y)
		allImpact = allImpact+1
	end)
	assert(allImpact == 60, allImpact)
end

return SynthesizeRange