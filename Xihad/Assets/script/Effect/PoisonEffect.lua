--- 
-- 作为Effect的控制逻辑Component
-- @module PoisonEffect
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local BaseEffect = require "BaseEffect"

local PoisonEffect = {
	damage = 0,
}

function PoisonEffect.new( o, object )
	inherit(o, PoisonEffect, BaseEffect)
	o:init(object)
	return o
end


function PoisonEffect:roundUpdate( currentTeam )
	if self:checkUpdate( currentTeam ) then
		print("doing PoisonEffect at " .. self.rounds)
		self.target:handleDamage(self, self.source, "M")
	end
end

return PoisonEffect




