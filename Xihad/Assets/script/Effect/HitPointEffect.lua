local base = require 'Effect.RoundEffect'
local HitPointEffect = setmetatable({
	inc = 0,
}, base)
HitPointEffect.__index = HitPointEffect

function HitPointEffect.new(inc, recycler)
	local effectType = inc > 0 and 'recover' or 'poison'
	local obj = setmetatable(base.new(effectType, recycler), HitPointEffect)
	obj.inc = inc
	
	return obj
end

function HitPointEffect:onRoundBegin()
	-- require camera focus
	local warrior = self:getBinding()
	local inc = self.inc
	if inc > 0 then
		warrior:takeRecovery(inc)
	elseif inc < 0 then
		warrior:takeDamage(-inc)
	end
	
	local AsConditionFactory = require 'Async.AsConditionFactory'
	AsConditionFactory.waitTimer(0.5)
end

return HitPointEffect