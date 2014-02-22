ElementManager = {
	
}
package.loaded["HeroManager"] = HeroManager

require "MapElement"

function ElementManager:new( o )
	o = o or {}
	o.elements = o.elements or {}
	o.currentCharacter = 0

	setmetatable(o, {__index = self})
	return o
end

function ElementManager:init(  )
	for i=1,10 do
		self.elements[#self.elements + 1] = 
		MapElement:new{
			type = "treasureBox"
		}
	end
end