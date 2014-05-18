local base = require 'Effect.BoundEffect'
local SleepEffect = setmetatable({}, base)
SleepEffect.__index = SleepEffect

function SleepEffect.new(inc, round)
	return setmetatable(base.new(round), SleepEffect)
end

function SleepEffect:onRoundBegin()
	-- require camera focus
	self:getBinding():deactivate()
	base.onRoundBegin()
end

return SleepEffect