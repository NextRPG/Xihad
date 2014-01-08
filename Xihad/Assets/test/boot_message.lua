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
	-- 妈的闭包竟然不能访问 self 
	local comp = self
	
	-- 初始化消息接收器，不能从构造函数中初始化
	self.testListener = {}
	local lsn = self.testListener
	function lsn:onMessage(src, param, tag)
		-- 不要随意使用全局的 scene ，在未来的版本中可能会移除
		local dsp = comp.object:getScene():dispatcher()
		if param == "kill component" then
			assert(comp.object:removeComponent(c"ListenerManager"))
			dsp:dispatch(c"test", "I wont receive this message", c"sb")
		elseif param =="kill listener" then
			assert(comp:removeDestroyable(lsn))
			assert(lsn:destroy())
			dsp:dispatch(c"test", "I wont receive this message", c"sb")
		else 
			print(string.format("Got message from %q with parameter %s", src:getID(), tostring(param)))
		end
	end
	
	self:attatchListener(lsn, "test")
end

function ListenerManager:attatchListener(lsn, ...)
	local dsp = self.object:getScene():dispatcher()
	
	function lsn:onDestroy()
		print("listener:onDestroy()")
		dsp:clearListener(lsn)
	end
	
	-- register listener
	for i=1,select('#', ...) do
		dsp:addListener(c(select(i, ...)), lsn)
	end
	
	-- 让消息接收器的生命期伴随这个组件
	self:appendDestroyable(lsn)
end

local dsp = scene:dispatcher()
local go = scene:createObject(c"sb")
local lm = go:appendComponent(c"ListenerManager")

dsp:dispatch(c"test", "test message", c"__ROOT__", 0.1)
-- dsp:dispatch(c"test", "kill component", c"__ROOT__", 0.15)
dsp:dispatch(c"test", "kill listener", c"__ROOT__", 0.15)

-- TODO 测试用例增加 dispatcher:remove()
