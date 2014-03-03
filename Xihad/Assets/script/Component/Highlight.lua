--- 
-- 能够以某种颜色显示高亮的Component
-- @module Highlight
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Stack = require "Stack"

local Highlight = {
	 BLUE = "102b6a",
	 RED = "aa2116",
	 PURPLE = "411445",
	 WHITE = "fffffb",
	 ALPHA = "ALPHA"
}

function Highlight.new( o )
	o = o or {}
	inherit(o, Highlight)

	o.Stack = Stack.new{}

	return o
end

function Highlight:change2Alpha(  )
	if self.object:findComponent(c"Mesh") then
		self.object:removeComponent(c"Mesh")
	end
end

function Highlight:changeColor(  )
	local Stack = self.Stack
	if not Stack:empty() then
		if Stack:top() == "ALPHA" then
			self:change2Alpha()
			return
		elseif not self.object:findComponent(c"Mesh") then
			local colorMesh = geometry:createCube(Consts.TILE_WIDTH - 1, 0.5, Consts.TILE_HEIGHT - 1)
			self.object:appendComponent(c"Mesh"):setMesh(colorMesh)
		end
		self.object:findComponent(c"Mesh"):setColor(hex2Color(Stack:top()))
	else
		self:change2Alpha()
	end
end

function Highlight:pushColor( colorstr )
	self.Stack:push(self[colorstr])
	self:changeColor()
end

function Highlight:popColor(  )
	if not self.Stack:empty() then
		self.Stack:pop()
		self:changeColor(  )
	end
end

function Highlight:clear(  )
	self.Stack:clear()
	self:change2Alpha()
end

return Highlight