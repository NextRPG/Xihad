local XihadMapTile = require 'Chessboard.XihadMapTile'

local CursorEventDispatcher = { 
	objectListener = nil,
	intersectionListener = nil,
}
CursorEventDispatcher.__index = CursorEventDispatcher

function CursorEventDispatcher.new(objectListener, intersectionListener)
	return setmetatable({
			objectListener = objectListener,
			intersectionListener = intersectionListener,
		}, CursorEventDispatcher)
end

function CursorEventDispatcher:onIntersection(intersection)
	if self.intersectionListener then
		self.intersectionListener:onIntersection(intersection)
	end
end

function CursorEventDispatcher:dispatch(x, y, ...)
	local ray = g_collision:getRayFromScreenCoord(x, y)
	
	if self.objectListener:needCollisionDetection() then
		local obj, intersection = g_collision:detect(ray)
		
		if obj and obj:hasTag(c'Warrior') then
			self:onIntersection(intersection)
			
			return self.objectListener:onWarrior(obj, ...)
		end
	end
	
	local intersection = XihadMapTile.intersectsGround(ray)
	local location = XihadMapTile.projectToLocation(intersection)
	local tile = g_chessboard:getTile(location)
	
	if tile ~= nil then
		self:onIntersection(intersection)
		return self.objectListener:onTile(tile, ...)
	end
end

return CursorEventDispatcher
