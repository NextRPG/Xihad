local StateMachine = {
}

function StateMachine.new( o )
	o = o or {}
	setmetatable(o, {__index = StateMachine})
	o.ASYNC = false
	o.STOP = false
	return o
end

function StateMachine:setInitial( state )
	assert(type(state) == "string" and self[state] ~= nil)
	self.currentState = state
end

function StateMachine:addState( state )
	assert(type(state) == "string" and self[state] == nil)
	self[state] = {}
end

function StateMachine:addTransition( state, nextState, condition, action )
	assert(type(state) == "string" and type(nextState) == "string" 
		and type(action) == "function"
		and type(condition) == "function" and self[state] ~= nil)
	self[state][condition] =
	function ( ... )
		action( ... )
		self.currentState = nextState
	end 
end

function StateMachine:update( ... )
	for condition, action in pairs(self[self.currentState]) do
		if condition( ... ) and not self.ASYNC and not self.STOP then
			self:pendingAndAction(action, ...)
			break;
		end
	end
end

function StateMachine:stop(  )
	self.STOP = true
end

function StateMachine:pendingAndAction( func, ... )
	self.ASYNC = true
	co = coroutine.create(function ( ... )
		func(...)
		self.ASYNC = false	
	end)
	runCallback(coroutine.resume(co, ...))
end

return StateMachine