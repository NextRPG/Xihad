--- 
-- 在地图上的陷阱，可以将敌人困住
-- @module Trap
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Trap = {

}

function Trap.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = Trap})

	return o
end

return Trap