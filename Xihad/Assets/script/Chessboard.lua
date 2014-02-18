--- 
-- 负责地图块TileObject的初始化，查询，标记
-- @module Chessboard
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local TerrainDatabase = require "TerrainDatabase"

---
-- 管理Tiile的表
local Chessboard = {
	
}

-- private

-- 按地形重新设置颜色
local function resetColor( tileObject )
	local tile = tileObject:findComponent(c"Tile")
	local fcomp = tileObject:findComponent(c"Mesh")
	if tile.terrain.id == 1 then
		-- light green
		fcomp:setColor(152, 241, 156)
	elseif tile.terrain.id == 2 then
		-- BLUE
		fcomp:setColor(7, 65, 255)
	elseif tile.terrain.id == 3 then
		-- deep green 
		fcomp:setColor(11, 110, 11)
	end
end

-- public

---
-- 创建一个TileObject
-- @tparam Tile tile
-- @treturn Object tileObject
function Chessboard:createTile( tile )
	local cubeMesh = geometry:createCube(Consts.TILE_WIDTH, 5, Consts.TILE_HEIGHT)
	local tileObject = scene:createObject(c(tile.x .. " " .. tile.y))
	fcomp = tileObject:appendComponent(c"Mesh")
	fcomp:setMesh(cubeMesh)
	fcomp:createSelector(c"stupid") 
	tileObject:appendComponent(c"Tile", tile)
	resetColor(tileObject)

	local location = getPixelLocation(tile)
	tileObject:concatTranslate(location.x, 0, location.z)

	tileObject:addTag(c"Tile")

	return tileObject
end

---
-- 对Chessboard进行初始化
-- @tab map 地图信息
-- @return nil
function Chessboard:init( map )
	for i,tile in ipairs(map) do
		local id = tile.terrain
		tile.terrain = TerrainDatabase[id]
		self:createTile(tile)
	end
	Consts.ROWS = map.height
	Consts.COLS = map.width
end

---
-- 查询TileObject
-- @tab location
-- @treturn Object tileObject
function Chessboard:tileAt( location )
	return scene:findObject(c(location.x .. " " .. location.y))
end

--- 
-- 查询地图块上是否有CharacterObject
-- @tab location
-- @return bool
function Chessboard:hasCharacter( location )
	local AIManager, HeroManager = require "AIManager", require "HeroManager"
	return AIManager:getCharacterByLocation(location) 
		or HeroManager:getCharacterByLocation(location)
end

---
-- 恢复一些地图块为原来的颜色
-- @tparam {Point,...} points
-- @return nil
function Chessboard:recoverArea( points )
	if points == nil then return end
	for k,point in pairs(points) do
		if type(point) == "table" then
			resetColor(self:tileAt(point)) 
		end
	end
end

---
-- 标记一些地图块为某种颜色
-- @tparam {Point,...} points
-- @tparam {r,g,b} color
-- @return nil
function Chessboard:markArea(points, color)
	if points == nil then return end
	color = color or {255, 0, 0}
	for k,point in pairs(points) do
		if type(point) == "table" then
			local mesh = self:tileAt(point):findComponent(c"Mesh")
			mesh:setColor(color[1], color[2], color[3])
		end
	end
end

return Chessboard