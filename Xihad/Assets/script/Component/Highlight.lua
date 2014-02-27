--- 
-- 能够以某种颜色显示高亮的Component
-- @module Highlight
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local stack = require "Stack"

local Highlight = {
	 BLUE = "102b6a",
	 RED = "aa2116",
	 PURPLE = "411445",
	 WHITE = "fffffb",
	 ALPHA = "ALPHA"
}

function Highlight.new( o )
	o = o or {}
	setmetatable(o, {__index = Highlight})

	o.stack = stack.new{}

	return o
end

function Highlight:change2Alpha(  )
	if self.object:findComponent(c"Mesh") then
		self.object:removeComponent(c"Mesh")
	end
end

function Highlight:changeColor(  )
	local stack = self.stack
	if not stack:empty() then
		if stack:top() == "ALPHA" then
			self:change2Alpha()
			return
		elseif not self.object:findComponent(c"Mesh") then
			local colorMesh = geometry:createCube(Consts.TILE_WIDTH - 1, 0.5, Consts.TILE_HEIGHT - 1)
			self.object:appendComponent(c"Mesh"):setMesh(colorMesh)
		end
		self.object:findComponent(c"Mesh"):setColor(hex2Color(stack:top()))
	else
		self:change2Alpha()
	end
end

function Highlight:pushColor( colorstr )
	self.stack:push(self[colorstr])
	self:changeColor()
end

function Highlight:popColor(  )
	if not self.stack:empty() then
		self.stack:pop()
		self:changeColor(  )
	end
end

function Highlight:clear(  )
	self.stack:clear()
	self:change2Alpha()
end

return Highlight