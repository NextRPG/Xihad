local RoundRecycler = {
	restRound = 0,
	_checkWhenBegin = false,
	effect = nil,
}
RoundRecycler.__index = RoundRecycler

function RoundRecycler.new(round, checkWhenBegin)
	assert(round > 0)
	return setmetatable({
			restRound = round,
			_checkWhenBegin = checkWhenBegin,
		}, RoundRecycler)
end

function RoundRecycler:copy()
	self:_checkAlive()
	
	self.__index = self
	return setmetatable({
			effect = nil,
		}, self)
end

function RoundRecycler:_destroy()
	self.effect = nil
	self.restRound = false
	self._checkWhenBegin = nil
end

function RoundRecycler:_isAlive()
	return self.restRound ~= false
end

function RoundRecycler:_checkAlive(msg)
	assert(self:_isAlive(), msg)
end

function RoundRecycler:start(effect, binding)
	self:_checkAlive('Attemp to rebind to another effect')
	
	assert(not self.effect)
	binding:addRoundListener(self)
	self.effect = effect
end

function RoundRecycler:stop(effect, binding)
	self:_checkAlive()
	
	if not self.effect then return end
	
	assert(effect == self.effect)
	binding:removeRoundListener(self)
	
	self:_destroy()
end

function RoundRecycler:onExhausted()
	self.effect:unbind()
	assert(not self:_isAlive(), self.restRound)
end

function RoundRecycler:afterExhausted()
	error('you may need to detach self from round updater')
end

function RoundRecycler:_check()
	if self.restRound < 0 then
		self:afterExhausted()
	elseif self.restRound == 0 then
		self:onExhausted()
	end
end

function RoundRecycler:_update()
	self.restRound = self.restRound - 1
	self:_check()
end

-- TODO invoke this in warrior
function RoundRecycler:onBattleEnd()
	assert(self.effect)
	if self.restRound > 0 then
		self.restRound = 0
		self:onExhausted()
	end
end

function RoundRecycler:onRoundBegin()
	assert(self.effect)
	if self._checkWhenBegin then
		self:_update()
	end
end

function RoundRecycler:onRoundEnd()
	assert(self.effect)
	if not self._checkWhenBegin then
		self:_update()
	end
end

return RoundRecycler