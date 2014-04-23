local sm = {
	current = nil,
	listeners  = nil,
	transitions = nil,
}
sm.__index = sm

function sm.new(initialState)
	local o = {}
	setmetatable(o, sm)
	o.current = initialState
	o.listeners = {}
	o.transitions = {}
	return o
end

function sm:getCurrentState()
	return self.current
end

function sm:isInState(state)
	return self.current == state
end

function sm:addStateListener(state, listener)
	if not self.listeners[state] then
		self.listeners[state] = {}
	end
	
	if not self.listeners[state][listener] then
		self.listeners[state][listener] = true
		listener:onStateEnter(self.current, nil)
	end
end

function sm:removeStateListener(state, listener)
	if not self.listeners[state] then return end
	
	self.listeners[state][listener] = nil
end

function sm:setTransition(fromState, msg, nextState)
	if fromState ~= nextState then 
		if not self.transitions[fromState] then
			self.transitions[fromState] = {}
		end
		
		self.transitions[fromState][msg] = nextState
	end
end

local function fireStateEnter(listeners, currentState, prevState)
	if not listeners then return end 
	
	for k,v in pairs(listeners) do
		assert(v)
		k:onStateEnter(currentState, prevState)
	end
end

local function fireStateExit(listeners, currentState, nextStateName)
	if not listeners then return end 
	
	for k,v in pairs(listeners) do
		assert(v)
		k:onStateExit(currentState, nextStateName)
	end
end

function sm:setCurrentState(nextState)
	if nextState == self.current then return false end
	
	local prevState = self.current
	fireStateExit(self.listeners[prevState], self.current, nextState)
	self.current = nextState
	fireStateEnter(self.listeners[nextState], self.current, prevState)
	return true
end

function sm:onMessage(msg)
	if not self.transitions[self.current] then
		print("no transitions in current state")
		return false
	end
	
	local nextState = self.transitions[self.current][msg]
	if nextState then 
		return self:setCurrentState(nextState) 
	end
	
	return false
end

if select('#', ...) == 0 then 
	local mysm = sm.new('1')
	mysm:setTransition("1", "to2", "2")
	mysm:setTransition("1", "to3", "3")
	mysm:setTransition("2", "back", "1")
	mysm:setTransition("3", "back", "1")
	
	ls = {
		counter = 0,
		onStateEnter = function(self, curr, prev)
			assert(prev == '2' or prev == '3' or not prev)
			self.counter = self.counter+1
		end,
		
		onStateExit = function(self, curr, next)
			assert(next == '2' or next == '3')
			self.counter = self.counter-1
		end
	}
	mysm:addStateListener('1', ls)
	mysm:addStateListener('1', ls)	-- won't add it duplicately
	
	assert(ls.counter == 1)
	assert(mysm:isInState("1"))
	
	assert(mysm:onMessage('to2'))
	assert(mysm:isInState("2"))
	assert(ls.counter == 0)
	
	assert(not mysm:onMessage('to3'))
	assert(mysm:isInState("2"))
	assert(ls.counter == 0)
	
	assert(mysm:onMessage('back'))
	assert(mysm:isInState("1"))
	assert(ls.counter == 1)
	
	mysm:removeStateListener('1', ls)
	assert(mysm:onMessage('to3'))
	assert(mysm:isInState("3"))
	assert(ls.counter == 1)
	
	assert(not mysm:onMessage('noSuchMsg'))
	assert(mysm:isInState("3"))
end

return sm
