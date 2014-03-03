--- 
-- 表示背包中的一个物品
-- @module Item
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Item = {

}

function Item.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = Item})

	return o
end

return Item