local XihadTile = require 'Chessboard.XihadMapTile'
local width, height = XihadTile.getTileDimension()
local thickness = XihadTile.getThickness()

local colorMesh = g_geometry:createCube(width-1, thickness, height-1)
g_meshManager:takeMesh("@colorCube", colorMesh)

-- local cubeMesh = g_geometry:createCube(width, 5, width)
-- g_meshManager:takeMesh("@chessboardCube", cubeMesh)
