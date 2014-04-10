local StateMachine = {
	currentState = nil,
	states = nil,
	ASYNC  = false,
	STOP   = false,
}
StateMachine.__index = StateMachine

function StateMachine.new( o )
	o = o or {}
	setmetatable(o, StateMachine)
	o.states = {}
	return o
end

function StateMachine:verifyState(state)
	if not self.states[state] then
		error("The specified state hasn't been added")
	end
end

function StateMachine:setInitial( state )
	self:verifyState(state)
	self.currentState = state
end

function StateMachine:addState( state )
	if self.states[state] ~= nil then
		error("Already added state")
	end
	
	self.states[state] = {}
end

function StateMachine:addTransition( state, nextState, condition, action )
	self:verifyState(state)
	self.states[state][condition] =
	function ( ... )
		action( ... )
		self.currentState = nextState
	end 
end

function StateMachine:update( ... )
	if not self.currentState or self.ASYNC or self.STOP then
		return
	end
	
	for condition, action in pairs(self.states[self.currentState]) do
		if condition( ... ) then
			self:pendingAndAction(action, ...)
			break
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