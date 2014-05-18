local functional = require 'std.functional'
local sCoroutine = require 'std.sCoroutine'
---
-- AsyncCondition 负责与其他协程通信，来检测是否需要继续执行当前协程
local ascondition = {
}
ascondition.__index = ascondition

function ascondition.new()
	local o = {}
	setmetatable(o, ascondition)
	return o
end

---
-- callback 函数被调用时即会返回当前协程，应该被注册给某个模块
-- 此函数返回后，当前协程即会 yield ，此函数的返回值即是 yield 参数。
-- 即返回给其他协程 resume 函数的值
function ascondition:hook(callback)
	error("AsyncCondition.hook: no implementation by default")
end

---
-- 检查是否满足条件，参数为 yield 函数的返回值，即其他协程调用 resume 
-- 的参数。
-- @return 第一个返回值必须是 boolean。
-- 			如果返回 true, 那么条件满足，第二个返回值将是 wait() 的返回值
-- 			如果返回 false,那么条件不满足，第二个返回值将是 yield() 的参数，即 resume() 的返回值
function ascondition:inspect(...) 
	return true
end

--- 条件满足后一定会调用此函数来释放某些资源
function ascondition:release()
	
end

function ascondition:wait()
	local running = coroutine.running()
	if not running then
		io.stderr:write('Attempt to wait from main thread\n')
		error(debug.traceback())
	end
	
	local pending = functional.bind1(sCoroutine.resume, running)
	local yielding = self:hook(pending)
	local result
	repeat
		result, yielding = self:inspect(coroutine.yield(yielding))
		if type(result) ~= "boolean" then
			error("The first return value from AsyncCondition.inspect should be boolean")
		end
	until result ~= false
	self:release()
	
	return yielding
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
	
	
	function onShowDialog()
		local inputACondition = ascondition.new()
		function inputACondition:hook(callback)
			trigger:setListener(callback)
		end
		
		function inputACondition:inspect(choice)
			return choice=='A', choice
		end
		
		function inputACondition:release()
			trigger:setListener(nil)
		end
		
		local dialog = { 
			"Creates a new coroutine, with body f.", 
			"f must be a Lua function. Returns a function that resumes the coroutine each time it is called.", 
			"Any arguments passed to the function behave as the extra arguments to resume.", 
			"Returns the same values returned by resume, except the first boolean. In case of error, propagates the error.", 
		}
		
		local current = 1
		while current <= #dialog do
			print(dialog[current], "Press A to continue...")
			current = current + 1
			inputACondition:wait()
		end
		
		os.exit(0)
	end
	
	local startDialog = coroutine.wrap(onShowDialog)
	startDialog()
	
	while true do
		-- Simulate input
		local input = string.char(string.byte('A') + math.random(0, 3))
		print("Main: ...", input)
		trigger:onUserChoosed(input)
	end
end

return ascondition
