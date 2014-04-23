local base = require 'route.MapTile'
local XTile = { 
	yOffset = 0,
	tileWidth = 10, 
	tileHeight = 10,
}
XTile.__index = XTile
setmetatable(XTile, base)

function XTile.new(location)
	local obj = base.new(location)
	setmetatable(obj, XTile)
	
	return obj
end

function XTile.setTileDimension(tileWidth, tileHeight)
	assert(tileWidth>0 and tileHeight>0)
	XTile.tileWidth = tileWidth
	XTile.tileHeight= tileHeight
end

function XTile.getTileDimension()
	return XTile.tileWidth, XTile.tileHeight
end

function XTile.setYOffset(y)
	XTile.yOffset = y	 
end

function XTile:getLeftBottomVector()
	local loc = self:getLocation()
	local originX = (loc.x-1) * XTile.tileWidth
	local originZ = (loc.y-1) * XTile.tileHeight
	
	return math3d.vector(originX, self.yOffset, originZ)
end

function XTile:getCenterVector()
	local leftBottom = self:getLeftBottomVector()
	local x, _, z = leftBottom:xyz()
	local center = leftBottom 	-- reuse the vector
	center:set(x+XTile.tileWidth/2, nil, z+XTile.tileHeight/2)
	return leftBottom
end

function XTile.projectToLocation(vector)
	local x, _, z = vector:xyz()
	return Location.new(math.floor(x/XTile.tileWidth), math.floor(z/XTile.tileHeight))
end

return XTile