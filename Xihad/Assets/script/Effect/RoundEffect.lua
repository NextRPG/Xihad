local base = require 'Effect.BoundEffect'
local RoundEffect = setmetatable({}, base)
RoundEffect.__index = RoundEffect

function RoundEffect.new(recycler)
	return setmetatable(base.new(recycler), RoundEffect)
end

function RoundEffect:onBind(warrior)
	warrior:addRoundListener(self)
end

function RoundEffect:onUnbind(warrior)
	warrior:removeRoundListener(self)
end

function RoundEffect:onRoundBegin()
end

function RoundEffect:onRoundEnd()
	
end

return RoundEffect