local Location = require 'route.Location'

local BaseImpactRange = {
	relativeLocationSet = nil,
}
BaseImpactRange.__index = BaseImpactRange

function BaseImpactRange.new(relativeLocationSet)
	assert(relativeLocationSet ~= nil)
	return setmetatable({
			relativeLocationSet = relativeLocationSet,
		}, BaseImpactRange)
end

function BaseImpactRange:isMultiTarget()
	return self.relativeLocationSet:size() > 1
end

function BaseImpactRange:canImpact(launcherCenter, impactCenter, location)
	local found = false
	self:traverse(launcherCenter, impactCenter, 
		function (impactLoc)
			if impactLoc == location then
				found = true
				return true
			end
		end)
	
	return found
end

function BaseImpactRange:_createTransform(launcherCenter, impactCenter)
	error('no implementation by default')
end

function BaseImpactRange:traverse(launcherCenter, impactCenter, f)
	local transform = self:_createTransform(launcherCenter, impactCenter)
	for _, impactLoc in self.relativeLocationSet:values() do
		impactLoc = transform(impactLoc)
		if impactCenter then
			impactLoc = impactCenter + impactLoc
		end
		
		if f(impactLoc) then return end
	end
end

return BaseImpactRange
