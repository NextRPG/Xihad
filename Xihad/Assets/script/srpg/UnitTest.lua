require 'core.Point2D'

local Chessboard = require 'Chessboard'

local world = Chessboard.new()
for i=1,10 do
	for j=1,12 do
		world:newTile(point(i, j), Terrain.new('草地'))
	end
end


for i=1,10 do
	for j=1,12 do
		local tile = world:getTile(point(i, j))
		
		local left = world:getTile(point(i-1, j))
		if left then
			world:addEdge(tile, left)
		end
		
		-- ...
	end
end

local heroes = {}
for i=1,10 do
	table.insert(heroes, PropertyHost.new())
end

local enemies = {}
for i=1,10 do
	table.insert(enemies, PropertyHost.new())
end


