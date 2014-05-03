local Barrier = {
	tile = nil
}
Barrier.__index = Barrier

function Barrier.new()
	local o = {}
	setmetatable(o, Barrier)
	return o
end

function Barrier:canPass( warrior )
	return true
end

function Barrier:canStay( warrior )
	return true
end

function Barrier:permitCasting( warrior, skill )
	return false
end

function Barrier:getActionPointCost( warrior )
	return 0
end

function Barrier:getTile( )
	return self.tile
end

---
-- 可以通过返回的键值从 MapTile 中取得 Barrier 。但是，必须保证
-- 键值唯一性。否则直接返回 nil
function Barrier:getOptUniqueKey( )
	return nil
end

function Barrier:setTile( tile ) 
	if self.tile == tile then return false end
	
	if self.tile then
		self.tile:onBarrierRemoved(self)
		self.tile = nil
	end
	
	self.tile = tile
	
	if self.tile then
		self.tile:onBarrierAdded(self, self:getOptUniqueKey())
	end
	
	return true
end

return Barrier
