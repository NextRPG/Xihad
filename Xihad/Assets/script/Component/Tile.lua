--- 
-- 地图上的一块，可以查询地图块能否通过，通过消耗，能否停留等等。
-- @module Tile
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local TerrainDatabase = require "TerrainDatabase"
local Chessboard = require "Chessboard"

local Tile = {
	x = 0,
	y = 0,
	terrain = nil,
}

function Tile.new( o )
	o = o or {}
	o.elements = o.elements or {}
	o.charater = o.charater or nil

	setmetatable(o, {__index = Tile})
	return o
end

function Tile:permitCasting( charater, skill )
	for i,v in ipairs(self.elements) do
		if v:permitCasting( charater, skill ) == false then
			return false
		end
	end
	return true
end

function Tile:canPass( )
	return self.terrain.canPass
end

function Tile:canStay( charater )
	for i,v in ipairs(self.elements) do
		if v:canStay(charater) == false then
			return false
		end
	end
	return true
end

function Tile:getAPCost( charater )
	return self.terrain.APcost
end

-- add Tile

function Tile:addElement( element )
-- TODO: check repeat
	self.elements[#self.elements + 1] = element 
	element.tile = self
end

function Tile:removeElement( id )
	
end

function Tile:setCharacter( character )
	--  Tile has no character on it or character has left         
	if self.character == nil or character == nil then
		self.character = character
		if character ~= nil then
			character.tile = self
		end
		return true
	end
	return false
end

function Tile:removeCharacter( id )
	-- body
end

return Tile
