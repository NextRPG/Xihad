local base = require 'Skill.ToComponentResolver'
local Location = require 'route.Location'
local RepelResolver = {
	centerOnly = true,
	repelDistance = 1,
}
RepelResolver.__index = RepelResolver
setmetatable(RepelResolver, base)

function RepelResolver.new(centerOnly, distance)
	local obj = setmetatable(base.new('Warrior'), RepelResolver)
	obj.centerOnly = centerOnly
	obj.repelDistance = math.floor(distance)
	return obj
end

function RepelResolver:_resolve(sourceWarrior, targetWarrior, relativeLoc, result)
	if relativeLoc ~= Location.new() and self.centerOnly then
		return 
	end
	
	result:addRepel(self.repelDistance)
end

return RepelResolver
