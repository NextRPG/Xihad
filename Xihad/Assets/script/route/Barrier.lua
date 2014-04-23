local Barrier = {
	tile = nil
}
Barrier.__index = Barrier

function Barrier.new()
	local o = {}
	setmetatable(o, Barrier)
	return o
end

function Barrier:canPass( character )
	return true
end

function Barrier:canStay( character )
	return true
end

function Barrier:permitCasting( character, skill )
	return false
end

function Barrier:getActionPointCost( character )
	return 0
end

function Barrier:getTile( )
	return self.tile
end

function Barrier:setTile( tile ) 
	if self.tile == tile then return false end
	
	if self.tile then
		self.tile = nil
		self.tile:onElementRemoved(self)
	end
	
	self.tile = tile
	
	if self.tile then
		self.tile:onElementAdded(self)
	end
	
	return true
end

return Barrier
