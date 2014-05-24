local BoundEffect = {
	_state = 'init',
	_uid = nil,
	_binding = nil,
	_recycler= nil,
}
BoundEffect.__index = BoundEffect

function BoundEffect.new(recycler)
	return setmetatable({
			_recycler = recycler,
		}, BoundEffect)
end

function BoundEffect:copy()
	assert(self._state == 'init', 'Illegal state to copy')
	
	self.__index = self
	return setmetatable({
			_state = 'init',
			_binding  = nil,
			_recycler = self._recycler and self._recycler:copy(),
		}, self)
end

function BoundEffect:onBind(binding)
end

function BoundEffect:onUnbind()
end

function BoundEffect:getBinding()
	return self._binding
end

function BoundEffect:isBound()
	return self._state == 'binding' and self._binding ~= nil
end

function BoundEffect:_generateUID(cause)
	return cause
end

function BoundEffect:bind(warrior, cause)
	assert(self._state ~= 'binded', 'Attempt to re-bind to another warrior')
	assert(not self:isBound())
	
	self._binding = warrior
	self._state = 'binding'
	
	self._uid = self:_generateUID(cause)
	warrior:registerEffect(self._uid, self)
	self:onBind(warrior)
	
	if self._recycler then
		-- 通知回收器开始计时
		self._recycler:start(self, warrior)
	end
	
end

function BoundEffect:unbind()
	-- 可能是由于unbind操作而主动脱离；
	-- 也可能是由于其他技能影响了该属性，而被动脱离。
	if self._state == 'unbinding' then
		return
	end
	
	assert(self:isBound(), 'no binding, please invoke bind() first')
	self._state = 'unbinding'
	
	print('unbind BoundEffect')
	local warrior = self:getBinding()
	warrior:unregisterEffect(self._uid, self)
	self:onUnbind(warrior)
	
	if self._recycler then
		-- 由于可能会主动解绑，所以需要通知回收器停止
		self._recycler:stop(self, warrior)
	end
	
	self._binding = nil
	self._state = 'binded'
end

return BoundEffect