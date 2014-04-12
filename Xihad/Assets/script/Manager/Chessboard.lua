--- 
-- 负责地图块TileObject的初始化，查询，标记
-- @module Chessboard
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Location = require 'Location'
local TerrainDatabase = require "TerrainDatabase"

---
-- 管理Tiile的表
local Chessboard = {
	
}

-- private

-- 按地形重新设置颜色
local function resetColor( tileObject )
	-- TODO
	local tile = tileObject:findComponent(c"MapTile")
	local fcomp = tileObject:findComponent(c"Mesh")
	
	for i = 1, fcomp:getMaterialCount() do
		local fmat = fcomp:getMaterial(i-1)	-- CAUTION!!!
		fmat:setColorMaterial('none')
		-- if tile.terrain.id == 1 then
		-- 	-- light green
		-- 	fmat:setDiffuseColor(Color.new(0xffb2d235))
		-- elseif tile.terrain.id == 2 then
		-- 	-- BLUE
		-- 	-- fmat:setDiffuseColor(Color.new(0xff102b6a))
		-- 	fmat:setDiffuseColor(Color.new(0xff121a2a))
		-- elseif tile.terrain.id == 3 then
		-- 	-- deep green 
		-- 	fmat:setDiffuseColor(Color.new(0xff225a1f))
		-- end
	end
end

local function tname( location )
	return location.x .. " " .. location.y
end

-- public

---
-- 创建一个TileObject
-- @tparam Tile tile
-- @treturn Object tileObject
function Chessboard:createTile( tile )
	local tileObject = g_scene:createObject(c(tname(tile)))
	local fcomp = tileObject:appendComponent(c"Mesh")
	fcomp:setMesh(g_meshManager:getMesh("@chessboardCube"))
	fcomp:createSelector(c"stupid") 
	tileObject:appendComponent(c"MapTile", Location.new(tile.x, tile.y))
	resetColor(tileObject)

	local location = getPixelLocation(tile)
	tileObject:concatTranslate(math3d.vector(location.x, 0, location.z))

	tileObject:addTag(c"Tile")

	local highlightObject = g_scene:createObject(c("highlight" .. tname(tile)))
	highlightObject:appendComponent(c"Highlight")
	highlightObject:resetTranslate(tileObject:getTranslate() + math3d.vector(0, 2.5, 0))
	highlightObject:addTag(c"Highlight")

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
	return g_scene:findObject(c(tname(location))):findComponent(c"MapTile")
end

function Chessboard:highlightAt( location )
	return g_scene:findObject(c("highlight" .. tname(location))):findComponent(c"Highlight")
end

--- 
-- 查询地图块上是否有CharacterObject
-- @tab location
-- @return bool
function Chessboard:hasCharacter( point )
	return Chessboard:tileAt(point).character
end

---
-- 弹出地图当前的颜色（彻底忘记）
-- @tparam {Point,...} points
-- @return nil
function Chessboard:popArea( points )

	if points == nil then return end

	for i,point in ipairs(points) do

		self:highlightAt(point):popColor()
	end
end

---
-- 标记一些地图块为某种颜色
-- @tparam {Point,...} points
-- @tparam {r,g,b} color
-- @return nil
function Chessboard:pushArea(points, color)
	if points == nil then return end
	assert(color ~= nil, "color can not be nil")
	for i,point in ipairs(points) do
		self:highlightAt(point):pushColor(color)
	end
end

function Chessboard:clearAll(  )
	for highlightObject in g_scene:objectsWithTag("Highlight") do
		highlightObject:findComponent(c"Highlight"):clear()
	end
end

--- 
-- listeners

function Chessboard:onTileChanged( newTile, oldTile, character )
	Chessboard:tileAt(newTile).character = character
	if oldTile == nil then return end
	Chessboard:tileAt(oldTile).character = nil
end

return Chessboard
