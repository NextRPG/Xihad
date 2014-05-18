local base = require 'Effect.BoundEffect'
local RoundEffect = setmetatable({
		eType = nil,
	}, base)
RoundEffect.__index = RoundEffect

function RoundEffect.new(eType, recycler)
	local o = setmetatable(base.new(recycler), RoundEffect)
	o.eType = eType
	return o
end

function RoundEffect:onBind(warrior)
	warrior:addRoundListener(self)
	
	if self.eType then
		warrior:attachEffect(self.eType, self)
	end
end

function RoundEffect:onUnbind()
	local warrior = self:getBinding()
	warrior:removeRoundListener(self)
	
	if self.eType then
		warrior:detachEffect(self.eType, self)
	end
end

function RoundEffect:onRoundBegin()
end

function RoundEffect:onRoundEnd()
	
end

return RoundEffect