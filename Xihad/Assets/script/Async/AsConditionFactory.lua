local AsyncCondition = require 'std.AsyncCondition'

local cond = {}

local function waitOrReturn(condition, wait)
	if wait == nil then
		wait = true
	end
	
	if wait then 
		condition:wait()
	else
		return condition
	end
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

function cond.waitMessage(message, wait)
	local o = AsyncCondition.new()
	local listenerWrapper = {}
	
	function o:hook(callback)
		listenerWrapper.onMessage = callback
		g_dispatcher:addListener(message, listenerWrapper)
		listenerWrapper:drop()
	end
	
	function o:release()
		g_dispatcher:removeListener(message, listenerWrapper)
	end
	
	return waitOrReturn(o, wait)
end

function cond.waitCommon(object, add, remove, wait)
	local o = AsyncCondition.new()
	
	local hooked
	function o:hook(callback)
		object[add](object, callback)
		hooked = callback
	end
	
	function o:inspect(arg)
		return object==arg
	end
	
	function o:release()
		assert(hooked)
		object[remove](object, hooked)
		hooked = nil
	end
	
	return waitOrReturn(o, wait)
end

function cond.waitAction(action, wait)
	return cond.waitCommon(action, 
		'addFinishListener', 'removeFinishListener', wait)
end

function cond.waitCameraAim(aimingControl, wait)
	return cond.waitCommon(aimingControl, 
		'addAimListener', 'removeAimListener', wait)
end

function cond.waitTargetModifier(modifier, wait)
	return cond.waitCommon(modifier,
		'addReachListener', 'removeReachListener', wait)
end

function cond.waitTimer(secs, wait)
	local o = AsyncCondition.new()
	function o:hook(callback)
		g_scheduler:schedule(callback, secs)
	end
	
	return waitOrReturn(o, wait)
end

return cond