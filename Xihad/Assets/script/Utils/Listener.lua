--- 
-- 一个metatable没有被改动过的Listener
-- @module Listener
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Listener = {

}

function Listener:onMessage( srcObject, param, msgTag )
	print("haha")
	local arr = string.split(msgTag, ".")
	local methodName = "on"
	for i,v in ipairs(arr) do
		methodName = methodName .. string.upperFirst(v)
	end
	if self[methodName] == nil then
		error("no corresponding method")
	end
	self[methodName](self, srcObject, param)
end

return Listener