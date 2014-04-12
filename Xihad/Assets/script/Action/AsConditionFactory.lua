local AsyncCondition = require 'std.AsyncCondition'

local cond = {}

local function waitOrReturn(condition, wait)
	wait = wait or true
	if wait then 
		condition:wait()
	else
		return condition
	end
end

function cond.waitAction(action, wait)
	local o = AsyncCondition.new()
	local hooked
	function o:hook(callback)
		action:addFinishListener(callback)
		hooked = callback
	end
	
	function o:inspect(arg)
		return action==arg
	end
	
	function o:release()
		assert(hooked)
		action:removeFinishListener(hooked)
		hooked = nil
	end
	
	return waitOrReturn(o, wait)
end

function cond.waitAnimation(animation, wait)
	local o = AsyncCondition.new()

	function o:hook(callback)
		animation:setEndCallback(callback)
	end

	function o:release(callback)
		animation:setEndCallback(nil)
	end
	
	return waitOrReturn(o, wait)
end

return cond