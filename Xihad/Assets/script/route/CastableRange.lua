-- 表示技能、道具
-- 释放范围，表示角色当前位置为中心的可以释放技能、道具的距离范围
-- 影响范围，表示技能释放后，以技能释放点为中心影响的格子范围
--[[
o x o
x o x
o x o
]]
local Location  = require 'Location'
local Algorithm = require 'std.Algorithm'
local Array		= require 'std.Array'

local Range = {
	minLaunchDistance = 1,
	maxLaunchDistance = 1,
	relativeImpactLocations = nil,
}
Range.__index = Range

function Range.new(minD, maxD)
	local r = {}
	if type(minD) == 'number' and type(maxD) == 'number' then
		r.minLaunchDistance = Algorithm.max(0, minD)
		r.maxLaunchDistance = Algorithm.max(minD, maxD)
	end
	
	r.relativeImpactLocations = {}
	setmetatable(r, Range)
	return r
end

--- Construct purpose only
function Range:addRelativeImpactLocation(loc)
	local idx = Array.bsearch(self.relativeImpactLocations, loc)
	if idx < 0 then
		Array.insert(self.relativeImpactLocations, -idx, loc:copy())
	end
end

function Range:getLaunchRange()
	return self.minLaunchDistance, self.maxLaunchDistance
end

function Range:canLaunch(target, launcherCenter)
	launcherCenter = launcherCenter or Location.new(0, 0)
	local d = target:distance(launcherCenter)
	return self.minLaunchDistance <= d and d <= self.maxLaunchDistance
end

function Range:traverseLaunchableLocations(launcherCenter, f)
	launcherCenter = launcherCenter or Location.new(0, 0)
	
	local offset = Location.new()
	for dist=self.minLaunchDistance, self.maxLaunchDistance do
		local xWeight
		for xWeight = -dist, dist do
			offset.x = xWeight
			offset.y = dist - math.abs(offset.x)
			f(launcherCenter+offset)
			
			if offset.y ~= 0 then
				offset.y = -offset.y
				f(launcherCenter+offset)
			end
		end
	end
end

function Range:canImpact(target, impactCenter)
	target = impactCenter and target-impactCenter or target
	
	return Array.bsearch(self.relativeImpactLocations, target)>0
end

function Range:traverseImpactLocations(impactCenter, f)
	for _,impactLoc in ipairs(self.relativeImpactLocations) do
		local loc
		if impactCenter then
			loc = impactCenter + impactLoc
		else
			loc = impactLoc:copy()
		end
		
		f(loc)
	end
end

function Range:traverseAllImpactLocations(launcherCenter, impactCenterFilter, f)
	local arr = {}
	
	self:traverseLaunchableLocations(launcherCenter, function(impactCenter)
		if impactCenterFilter and not impactCenterFilter(impactCenter) then
			return
		end
		
		self:traverseImpactLocations(impactCenter, function(impactLocation)
			local idx = Array.bsearch(arr, impactLocation)
			if idx < 0 then
				Array.insert(arr, -idx, impactLocation)	-- bad performance
				f(impactLocation:copy())
			end
		end)
	end)
end

if select('#', ...) == 0 then 
	--[[
	o o o o o = o o o o o
	o o o o = x = o o o o
	o o o = x x x = o o o
	o o = x x x x x = o o
	o = x x x = x x x = o
	= x x x = N = x x x =
	o = x x x = x x x = o
	o o = x x x x x = o o
	o o o = x x x = o o o
	o o o o = x = o o o o
	o o o o o = o o o o o
	--]]
	local range = Range.new(2, 4)
	range:addRelativeImpactLocation(Location.new( 0, 0))
	range:addRelativeImpactLocation(Location.new( 0, 1))
	range:addRelativeImpactLocation(Location.new( 0,-1))
	range:addRelativeImpactLocation(Location.new( 1, 0))
	range:addRelativeImpactLocation(Location.new(-1, 0))
	
	assert(range:canLaunch(Location.new(2, 0)))
	assert(range:canLaunch(Location.new(0, 1), Location.new(2, 1)))
	assert(range:canLaunch(Location.new(3, 0)))
	assert(range:canLaunch(Location.new(0, 4)))
	assert(not range:canLaunch(Location.new(1, 4)))
	assert(not range:canLaunch(Location.new(1, 0)))
	
	assert(range:canImpact(Location.new( 1,  0)))
	assert(range:canImpact(Location.new( 0,  1)))
	assert(range:canImpact(Location.new( 0,  0)))
	assert(range:canImpact(Location.new( 0,  0), Location.new(1, 0)))
	assert(not range:canImpact(Location.new( 0,  0), Location.new(2, 0)))
	assert(not range:canImpact(Location.new( 1,  1)))
	
	local launchableCount = 0
	local launcherLoc = Location.new(1, 1)
	range:traverseLaunchableLocations(launcherLoc, function(loc)
		assert(range:canLaunch(loc, launcherLoc))
		launchableCount = launchableCount+1
	end)
	assert(launchableCount==36)
	
	local impactCount = 0
	local launchLoc = Location.new(1, 2)
	range:traverseImpactLocations(launchLoc, function (loc)
		assert(range:canImpact(loc, launchLoc))
		impactCount = impactCount+1
	end)
	assert(impactCount == 5)
	
	-- 10 + (9+7+5+3+1)*2 == 60
	local allImpact = 0
	range:traverseAllImpactLocations(Location.new(6, 6), nil, function(x, y)
		allImpact = allImpact+1
	end)
	assert(allImpact == 60, allImpact)
end

return CastableRange
