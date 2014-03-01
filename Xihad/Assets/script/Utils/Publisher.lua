--- 
-- 实现观察者模式中的被观察对象的方法(抽象类)
-- @module Publisher
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Set = require "Set"

local Publisher = {
	subscribers = {}
}

---
--@string key 要监听变化的变量
--@tparam subscriber
--@func callback
function Publisher:attachSubscriber( key, subscriber, callback )
	if self.subscribers[key] == nil then

		self.subscribers[key] = {}

	end
	self.subscribers[key][callback] = subscriber 
end

function Publisher:detachSubscriber( key, subscriber, callback )
	if self.subscribers[key] == nil then
		error("the key does not exist")
	end
	self.subscribers[key][callback] = nil
end

function Publisher:changeState( key, value )
	self[key] = value
	if self.subscribers[key] == nil then return end
	for callback,subscriber in pairs(self.subscribers[key]) do
		callback(subscriber, value)
	end
end

return Publisher