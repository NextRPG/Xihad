local base = require 'Effect.RoundEffect'
local SleepEffect = setmetatable({}, base)
SleepEffect.__index = SleepEffect

function SleepEffect.new(recycler)
	return setmetatable(base.new(recycler), SleepEffect)
end

function SleepEffect:_generateUID()
	return 'sleep'
end

function SleepEffect:onRoundBegin()
	-- require camera focus
	if self:getBinding():isActive() then
		self:getBinding():deactivate()
	end
end

return SleepEffect