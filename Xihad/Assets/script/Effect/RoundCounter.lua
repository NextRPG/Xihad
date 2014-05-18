local RoundCounter = {
	restRound = 0,
	_checkWhenBegin = true,
}
RoundCounter.__index = RoundCounter

function RoundCounter.new(round, checkWhenBegin)
	assert(round > 0)
	return setmetatable({
			restRound = round,
			_checkWhenBegin = checkWhenBegin,
		}, RoundCounter)
end

function RoundCounter:onExhausted()
	error('no default implementation by default')
end

function RoundCounter:afterExhausted()
	error('you may need to detach self from round updater')
end

function RoundCounter:_check()
	if self.restRound < 0 then
		self:afterExhausted()
	elseif self.restRound == 0 then
		self:onExhausted()
	end
end

function RoundCounter:_update()
	self.restRound = self.restRound - 1
	self:_check()
end

function RoundCounter:onBattleEnd()
	if self.restRound > 0 then
		self.restRound = 0
		self:onExhausted()
	end
end

function RoundCounter:onRoundBegin()
	if self._checkWhenBegin then
		self:_update()
	end
end

function RoundCounter:onRoundEnd()
	if not self._checkWhenBegin then
		self:_update()
	end
end

return RoundCounter