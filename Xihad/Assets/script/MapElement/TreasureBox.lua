--- 
-- 表示地图上一个财宝箱，里面有物品
-- @module TreasureBox
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local TreasureBox = {

}

function TreasureBox.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = TreasureBox})

	return o
end

return TreasureBox