local XihadMapTile = require 'Chessboard.XihadMapTile'

local CursorEventDispatcher = { 
	listener = nil,
}
CursorEventDispatcher.__index = CursorEventDispatcher

function CursorEventDispatcher.new(listener)
	return setmetatable({
			listener = listener,
		}, CursorEventDispatcher)
end

function CursorEventDispatcher:dispatch(x, y)
	local ray = g_collision:getRayFromScreenCoord(x, y)
	
	if self.listener:needCollisionDetection() then
		local obj = g_collision:detect(ray)
		
		if obj and obj:hasTag(c'Warrior') then
			return self.listener:onWarrior(obj)
		end
	end
	
	local intersection = XihadMapTile.intersectsGround(ray)
	local location = XihadMapTile.projectToLocation(intersection)
	local tile = g_chessboard:getTile(location)
	
	if tile ~= nil then
		return self.listener:onTile(tile)
	end
end

return CursorEventDispatcher
