local base = require 'Skill.ToWarriorResolver'
local Location  = require 'route.Location'
local Algorithm = require 'std.Algorithm'
local HitPointResolver = {
	nature = 'unkonwn', 
	attenuation = 0,
	hitPointIncr= nil,
	
	sNaturePromote = {},
}
HitPointResolver.__index = HitPointResolver
setmetatable(HitPointResolver, base)

function HitPointResolver.new(nature, hitPointIncr, attenuation)
	local obj = setmetatable(base.new(), HitPointResolver)
	obj.nature = nature
	obj.hitPointIncr = hitPointIncr
	obj.attenuation  = attenuation	
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

function HitPointResolver:_resolve(
		sourceWarrior, targetWarrior, relativeLoc, result)
	local distance = relativeLoc:distance(Location.new())
	local ratio = math.max(0, 1 - distance * self.attenuation)
		
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
	
	if harmful and hitPointIncr <= 0 then
		result:addDamage(-hitPointIncr, 1)
	elseif not harmful and hitPointIncr >= 0 then
		result:addRecovery(hitPointIncr, 1)
	end
end

return HitPointResolver
