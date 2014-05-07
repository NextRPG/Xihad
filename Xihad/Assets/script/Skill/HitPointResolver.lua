local base = require 'Skill.ToWarriorResolver'
local Algorithm = require 'std.Algorithm'
local HitPointResolver = {
	nature = nil,
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

function HitPointResolver.getAddition(skillNature, warriorNature)
	local t = HitPointResolver.sNaturePromote[self.nature]
	
	return t and t[warriorNature] or 1
end

function HitPointResolver:_resolve(sourceWarrior, targetWarrior, relativeLoc, result)
	local distance = relativeLoc:distance(Location.new())
	local ratio = Algorithm.max(0, 1 - distance * self.attenuation)
		
	local hitPointIncr = self.hitPointIncr
	local harmful = hitPointIncr <= 0
	if harmful then
		local atk = sourceWarrior:getATK()
		local dfs = targetWarrior:getDFS()
		hitPointIncr = -Algorithm.max(0, (atk - dfs)) + hitPointIncr
		
		local addition = HitPointResolver.getAddition(self.nature, targetWarrior:getNature())
		hitPointIncr = hitPointIncr * addition
	end
	
	hitPointIncr = hitPointIncr * ratio
	
	if harmful and hitPointIncr <= 0 then
		result.damage = -hitPointIncr
	elseif not harmful and hitPointIncr >= 0 then
		result.recovery = hitPointIncr
	end
end

return HitPointResolver
