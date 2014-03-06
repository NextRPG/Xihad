local BattleManager = require "BattleManager"

local BaseEffect = {
	rounds = 0, 
	source = {},
	target = {}
}

function BaseEffect:init( object )
	object:setParent(self.target.object)
	BattleManager:attachSubscriber( "round", self, self.roundUpdate)
end

function BaseEffect:checkUpdate( currentTeam )
	if self.rounds ~= 0 and currentTeam == self.target.team then
		self.rounds = self.rounds - 1
		return true
	elseif self.rounds == 0 then
		-- self.object:stop()
	end
	return false
end

function BaseEffect.checkAvailable( newEffect, target )
	local oldEffect = scene:findObject(c(BaseEffect.makeName(newEffect, target)))

	if oldEffect then
		oldEffect = oldEffect:findComponent(c(newEffect.name))
		for k,v in pairs(newEffect) do
			if type(v) == "number" then
				if oldEffect.onStop then oldEffect:onStop() end
				oldEffect[k] = oldEffect[k] and math.max(oldEffect[k], newEffect[k]) or newEffect[k]
				if oldEffect.onStart then oldEffect:onStart() end
			end
		end
		oldEffect.changed = true
		return false
	else
		return true
	end
end

function BaseEffect.makeName( effect, target )
	return effect.name .. target.name
end

return BaseEffect