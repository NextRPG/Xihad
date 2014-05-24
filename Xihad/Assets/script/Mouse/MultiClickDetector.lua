local functional = require 'std.functional'
local MultiClickDetector = {
	maxTimes = -1,
	lastClickX = 0,
	lastClickY = 0,
	clickTimes = 0,
	listeners = nil,
	pressType = nil,
	scheduler = nil,
	
	maxMouseMove = 3,
	successiveTime = 0.2,
}
MultiClickDetector.__index = MultiClickDetector

function MultiClickDetector.new(scheduler, pressType, maxTimes)
	return setmetatable({
			listeners = {},
			pressType = pressType,
			scheduler = scheduler,
			maxTimes  = maxTimes,
		}, MultiClickDetector)
end

function MultiClickDetector:_moved(x, y)
	return math.abs(self.lastClickX - x) > self.maxMouseMove 
		or math.abs(self.lastClickY - y) > self.maxMouseMove
end

function MultiClickDetector:_schedule(timeout)
	local callback = functional.bindself(self, '_timeout')
	self.task = self.scheduler:schedule(callback, timeout)
end

function MultiClickDetector:_continue()
	self.scheduler:adjustTask(self.task, self.successiveTime)
end

function MultiClickDetector:_updateTask()
	if self.clickTimes == 1 then
		assert(not self.task)
		self:_schedule(self.successiveTime)
	else
		assert(self.task and not self.task:hasStarted())
		self:_continue()
	end
end

function MultiClickDetector:_exceedsMaxTimes()
	return self.maxTimes > 0 and self.clickTimes >= self.maxTimes
end

function MultiClickDetector:onMouseEvent(e)
	if e.type ~= self.pressType then
		return 1
	end
	
	local x, y = g_cursor:getPosition()
	local finished = true
	if self.clickTimes == 0 or not self:_moved(x, y) then
		self.clickTimes = self.clickTimes + 1
		finished = self:_exceedsMaxTimes()
	end
	
	if self.maxTimes == 1 then
		-- Optimize when maxTimes == 1
		assert(finished and self.clickTimes == 1)
		self:_schedule(0)
	elseif finished then
		assert(self.task and not self.task:hasStarted())
		self.scheduler:adjustTask(self.task, 0)
	else
		self.lastClickX, self.lastClickY = x, y
		self:_updateTask()
	end
	
	return 0
end

function MultiClickDetector:_timeout()
	assert(self.clickTimes ~= 0)
	
	-- at least clicked one time
	local times = self.clickTimes
	self.clickTimes = 0
	self.task = nil
	
	self:_fireClickEvent(times)
end

function MultiClickDetector:_fireClickEvent(times)
	local x, y  = self.lastClickX, self.lastClickY
	
	-- print('MultiClickDetector: ', times)
	local theListeners = self.listeners[times]
	if theListeners then
		for lis, _ in pairs(theListeners) do
			lis(self, times, x, y)
		end
	end
end

function MultiClickDetector:addClickTimesListener(times, lis)
	times = math.floor(times)
	if self.listeners[times] == nil then
		self.listeners[times] = {}
	end
	
	self.listeners[times][lis] = true
end

function MultiClickDetector:removeClickTimesListener(times, lis)
	times = math.floor(times)
	if lis and self.listeners[times] ~= nil then
		self.listeners[times][lis] = nil
	end
end

return MultiClickDetector