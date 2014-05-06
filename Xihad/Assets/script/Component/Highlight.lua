--- 
-- 能够以某种颜色显示高亮的Component
-- @module Highlight
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Stack = require "std.Stack"

local Highlight = {
	 BLUE 	= Color.new(0xff102b6a),
	 RED 	= Color.new(0xffaa2116),
	 PURPLE = Color.new(0xff411445),
	 WHITE 	= Color.new(0xfffffffb),
	 ALPHA 	= "ALPHA"
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
			local meshnode = self.object:appendComponent(c"Mesh")
			meshnode:setMesh(g_meshManager:getMesh("@colorCube"))
		end
		
		local mat = self.object:findComponent(c"Mesh"):getMaterial(0)
		mat:setDiffuseColor(Stack:top())
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