--- 
-- Set类用来存储在table中只能出现一次的数据
-- @module Set
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Set = {

}

function Set.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = Set})

	return o
end

function Set:add( e )
	self[e] = true
end

function Set:remove( e )
	self[e] = nil
end

function Set:empty(  )
	return next(self) == nil
end

return Set