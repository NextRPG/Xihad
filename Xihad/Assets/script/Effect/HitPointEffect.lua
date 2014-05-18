local base = require 'Effect.BoundEffect'
local HitPointEffect = setmetatable({
	inc = 0,
}, base)
HitPointEffect.__index = HitPointEffect

function HitPointEffect.new(inc, round)
	local obj = setmetatable(base.new(round), HitPointEffect)
	obj.inc = inc
	
	return obj
end

function HitPointEffect:doAffect()
	local warrior = self:getBinding()
	if self.inc > 0 then
		warrior:takeRecovery(inc)
	elseif self.inc < 0 then
		warrior:takeRecovery(-inc)
	end
end

function HitPointEffect:onRoundBegin()
	-- require camera focus
	self:doAffect()
	base.onRoundBegin()
end

return HitPointEffect