---
-- 严格区分两种类型的 MessageListener
-- 1. 属于 Component 的一部分，只有等到 Component stop 的时候才会失效
-- 2. 自治体，生命期完全由 Dispatcher 掌握，一旦不再接收任何类型的消息就失效
-- 第一种情况需要在对应的 Component 的 onStop() 方法中调用 drop()
-- 第二种情况需要在创建出 MessageListener 之后调用 drop()
local ListenerManager = {}
ListenerManager.__index = ListenerManager

-- 这样就可以不需要单独写一个 ListenerManager.lua 文件了
package.loaded["ListenerManager"] = ListenerManager
function ListenerManager.new(p, obj)
	local res = {}
	setmetatable(res, ListenerManager)
	
	return res
end

function ListenerManager:onStart()
	-- 初始化消息接收器，不能从构造函数中初始化
	self.testListener = {
		onMessage = function(self, srcObject, param, msgTag)
			print("Got message: ", param)
		end
	}
	
	local dispatcher = self.object:getg_scene():getDispatcher()
	dispatcher:addListener("test.a", self.testListener)
	
end

function ListenerManager:onStop()
	-- 不要随意使用全局的 g_scene ，在未来的版本中可能会移除
	local dispatcher = self.object:getg_scene():getDispatcher()
	
	-- dispatcher:removeListener("test", self.testListener)
	dispatcher:clearListener(self.testListener)
	self.testListener:drop()
end

local dispatcher = g_scene:getDispatcher()
local go = g_scene:createObject(c"sb")
local lm = go:appendComponent(c"ListenerManager")

dispatcher:dispatch("test.a", "msg param", c"root", 0.1)
dispatcher:dispatch("test.a", { param = "second" }, c"root", 0.15)
print("message deliverd");
