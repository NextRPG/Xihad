local functional = require("functional")
local thread = {}

function thread.entrust(entruster, m, ...)
	local running = assert(coroutine.running())
	local inTrust = functional.bind1(coroutine.resume, running)

	if type(entruster) == "function" then
		entruster(inTrust)
		return coroutine.yield(m, ...)
	else --if type(entruster) == "table" then
		entruster[m](entruster, inTrust)
		return coroutine.yield(...)
	end
end

if select('#', ...) == 0 then 
	local trigger = { listener = nil }
	function trigger.onUserChoosed(self, choice)
		if self.listener then
			self.listener(choice)
		end
	end
	
	function trigger.setListener(self, l)
		self.listener = l
	end
	
	local t1 = coroutine.create(function ()
		print("	AI-1: start")
		
		print("	AI-1: I need to know user's choice")
		local choice
		repeat
			choice = thread.entrust(trigger, "setListener")
		until choice
		
		print("	AI-1: now process user choice: ", choice)
		print("	AI-1: finish")
	end)
	
	for i=1,10 do
		print("Main: ...")
		coroutine.resume(t1)
		
		-- Simulate interrupt
		if i == 5 then
			print("Main: user choosed 'A' in main thread")
			trigger:onUserChoosed("A")
		end
	end
	print("Main: exit program")
end

return thread