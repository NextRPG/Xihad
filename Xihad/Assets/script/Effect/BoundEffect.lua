local BoundEffect = {
	_state = 'init',
	_uid = nil,
	_source  = nil,
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

function BoundEffect:_prepare_bind(bindPoint)
	assert(self._state == 'init', 'Attempt to re-bind to another bindPoint')
	
	self._binding = bindPoint
	self._state = 'binding'
end

function BoundEffect:_after_bind()
	self:onBind(self._binding)
	
	if self._recycler then
		self._recycler:start(self, self._binding)
	end
end

function BoundEffect:bindSticky(bindPoint, source)
	self:_prepare_bind(bindPoint)
	
	assert(source ~= nil)
	self._source = source
	bindPoint:registerStickyEffect(self, source)
	
	self:_after_bind()
end

function BoundEffect:bindExclusive(bindPoint, cause)
	self:_prepare_bind(bindPoint)
	
	self._uid = self:_generateUID(cause)
	assert(self._uid ~= nil, "uid shouldn't be nil")
	
	bindPoint:registerExclusiveEffect(self, self._uid)
	
	self:_after_bind()
end

function BoundEffect:unbind()
	assert(self:isBound(), 'no binding, please invoke bind*() first')
	
	self._state = 'unbinding' -- assert(not self:isBound())
	
	local bindPoint = self:getBinding()
	
	if self._uid then
		assert(self._source == nil)
		bindPoint:unregisterExclusiveEffect(self, self._uid)
		self._uid = nil
	else
		assert(self._source ~= nil)
		bindPoint:unregisterStickyEffect(self)
		self._source = nil
	end
	
	self:onUnbind(bindPoint)
	
	if self._recycler then
		self._recycler:stop(self, bindPoint)
	end
	
	self._binding = nil
	self._state = 'unbinded'
end

return BoundEffect