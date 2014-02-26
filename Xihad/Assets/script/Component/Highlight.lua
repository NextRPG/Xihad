--- 
-- 能够以某种颜色显示高亮的Component
-- @module Highlight
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local stack = require "Stack"

local Highlight = {
	 BLUE = "2a5caa",
	 RED = "aa2116",
	 PURPLE = "411445",
	 WHITE = "fffffb",
}

function Highlight.new( o )
	o = o or {}
	setmetatable(o, {__index = Highlight})

	o.stack = stack.new{}

	return o
end

function Highlight:changeColor(  )
	local stack = self.stack
	if not stack:empty() then
		if not self.object:findComponent(c"Mesh") then
			self.object:appendComponent(c"Mesh")
		end
		self.object:findComponent(c"Mesh"):setColor(stack:top())
	elseif self.object:findComponent(c"Mesh") then
		self.object:removeComponent(c"Mesh")
	end
end

function Highlight:pushColor( colorstr )
	self.stack:push(self[colorstr])
	self:changeColor()
end

function Highlight:popColor( colorstr )
	if not self.stack:empty() then
		self.stack:pop()
		self.changeColor(  )
	end
end

return Highlight