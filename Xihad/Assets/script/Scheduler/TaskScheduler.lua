local Heap = require 'std.Heap'
local Array= require 'std.Array'
local Task = require 'Scheduler.Task'

local MinHeap = Heap.MinHeap
local TaskScheduler = {
	taskId= 0,
	tasks = nil,
}
TaskScheduler.__index = TaskScheduler

function TaskScheduler.new()
	return setmetatable({
			tasks = {},
		}, TaskScheduler)
end

function TaskScheduler:runOnMainThread(callback)
	self:schedule(callback)
end

function TaskScheduler:schedule(callback, delay)
	delay = delay or 0
	
	local task = Task.new(self.taskId, g_time.global + delay, callback)
	self.taskId = self.taskId + 1
	
	MinHeap:insert(self.tasks, task)
	return task
end

function TaskScheduler:cancel(task)
	if task:hasStarted() then
		error('Task has already started')
	end
	
	local index = MinHeap:find(self.tasks, task)
	
	if not index then 
		error('Task not found')
	end
	
	if self.tasks[index] ~= task then
		error('Error task')
	end
	
	MinHeap:remove(self.tasks, index)
end

function TaskScheduler:adjustTask(task, delay)
	self:cancel(task)
	task:_resetTime(g_time.global + delay)
	MinHeap:insert(self.tasks, task)
end

function TaskScheduler:onUpdate()
	assert(coroutine.running() == nil, 'Scheduler must run no main thread')
	
	local tasks = self.tasks
	local current = g_time.global
	while not Array.empty(tasks) do
		local task = Array.getFront(tasks)
		
		if task:hasExpired(current) then
			assert(MinHeap:remove(tasks) == task)
			task:start()
		else
			break
		end
	end
end

return TaskScheduler