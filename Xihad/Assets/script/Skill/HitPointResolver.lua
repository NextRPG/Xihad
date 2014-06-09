local base = require 'Skill.ToComponentResolver'
local Location  = require 'route.Location'
local Algorithm = require 'std.Algorithm'
local HitPointResolver = {
	nature = 'unkonwn', 
	attenuation = 0,
	hitPointIncr= nil,
	probabilty  = 1,
	
	sNaturePromote = {},
}
HitPointResolver.__index = HitPointResolver
setmetatable(HitPointResolver, base)

function HitPointResolver.new(nature, hitPointIncr, attenuation, probabilty)
	local obj = setmetatable(base.new('Warrior'), HitPointResolver)
	obj.nature = nature
	obj.hitPointIncr = hitPointIncr
	obj.attenuation  = attenuation
	obj.probabilty   = probabilty
	return obj
end

function HitPointResolver.registerAddition(skillNature, warriorNature, addition)
	assert(type(addition) == 'number')
	local t = HitPointResolver.sNaturePromote[skillNature]
	if not t then
		t = {}
		HitPointResolver.sNaturePromote[skillNature] = t
	end
	
	t[warriorNature] = addition
 end

function HitPointResolver.getAddition(skillNature, warriorNature)
	local t = HitPointResolver.sNaturePromote[skillNature]
	
	if t then
		local addition = t[warriorNature]
		if addition then
			return addition
		end
	end
	
	return 1
end

function HitPointResolver:_getRatio(relativeLoc)
	local distance = relativeLoc:distance(Location.new())
	return 1 - distance * self.attenuation
end

function HitPointResolver:_resolve(
		sourceWarrior, targetWarrior, relativeLoc, result)
	local ratio = self:_getRatio(relativeLoc)
	if ratio <= 0 then
		return 
	end
	
	local hitPointIncr = self.hitPointIncr
	local harmful = hitPointIncr <= 0
	if harmful then
		local atk = sourceWarrior:getATK()
		local dfs = targetWarrior:getDFS()
		hitPointIncr = -math.max(0, (atk - dfs)) + hitPointIncr
		
		local targetNature = targetWarrior:getNature()
		local addition = HitPointResolver.getAddition(self.nature, targetNature)
		hitPointIncr = hitPointIncr * addition
	end
	
	hitPointIncr = hitPointIncr * ratio
	
	if harmful then
		result:addDamage(self.probabilty, -hitPointIncr)
	else
		result:addRecovery(self.probabilty, hitPointIncr)
	end
end

return HitPointResolver
