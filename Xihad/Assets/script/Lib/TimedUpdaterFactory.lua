local tuf = {}

function tuf.sequence(...)
	local s = {...}
	local i = 1
	local idle = 0
	function s.onUpdate(self, time)
		local t = self[i]
		local finish
		if type(t) == "number" then
			idle = idle + time
			finish = idle >= t
		else
			t:update(time)
			finish = t:stopped()
		end
		
		if finish then
			i = i + 1
			idle = 0
		end
	end
	
	function s.stopped(self)
		return i > #self
	end
	
	function s.reset(self)
		for i=1,i do
			self[i]:reset()
		end
		
		i = 1
	end
	
	return s
end

function tuf.parallel(...)
	local p = {...}
	local allstopped = false
	
	function p.onUpdate(self, time)
		for _,t in ipairs(self) do
			t:update(time)
			allstopped = allstopped and t:stopped()
		end
	end
	
	function p.reset(self)
		for _,t in ipairs(self) do
			t:reset()
		end
		
		allstopped = false
	end
	
	function p:stopped()
		return allstopped
	end
	
	return p
end

function tuf.loop(t, n)
	local l = {}
	local looped = 0
	function l.onUpdate(self, time)
		t:update(time)
		if t:stopped() then
			t:reset()
			looped = looped+1
		end
	end
	
	function l.reset(self)
		t:reset()
		looped = 0
	end
	
	function l:stopped()
		return looped >= n
	end
end

if select('#', ...) == 0 then 
	package.path = "../?.lua;./?.lua"
	local Timed = require "TimedUpdater"
	local tskgrp = {}
	
	for i=1,5 do
		local task = Timed.new(i)
		task.result = 0
		function task.onProgress(self, pro)
			self.result = pro
			print('', i, self.result)
		end
		
		table.insert(tskgrp, task)
	end
	
	local seq = tuf.sequence(unpack(tskgrp))
	
	local tm = 0
	local dt = 0.1
	while true do
		tm = tm + dt
		print("global time: ", tm)
		seq:update(dt)
		if tm > 20 then
			break
		end
	end
end

return tuf