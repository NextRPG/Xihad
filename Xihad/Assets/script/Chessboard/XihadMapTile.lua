local base = require 'route.MapTile'
local Terrain  = require 'Barrier.Terrain'
local Location = require 'route.Location'
local Rectangle= require 'math.Rectangle' 
local WarriorBarrier = require 'WarriorBarrier'

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

function XTile.projectToLocation(vector)
	local x, _, z = vector:xyz()
	return Location.new(math.floor(x/XTile.tileWidth)+1, math.floor(z/XTile.tileHeight)+1)
end

local ground = math3d.plane(math3d.vector(), math3d.vector(0, 1, 0))
function XTile.intersectsGround(ray)
	local met, ratio = math3d.intersects(ground, ray)
	return ray:start() + ray:vector()*ratio
end

function XTile:getWarrior()
	local barrier = self:findBarrierByKey(WarriorBarrier.getOptUniqueKey())

	if barrier then
		return barrier:findPeer(c'Warrior')
	end
end

function XTile:hasWarrior()
	return self:getWarrior() ~= nil
end

function XTile:getTerrain()
	return self:findBarrierByKey(Terrain.getOptUniqueKey())
end

function XTile:getLeftBottomVector()
	local loc = self:getLocation()
	local originX = (loc.x-1) * XTile.tileWidth
	local originZ = (loc.y-1) * XTile.tileHeight
	
	return math3d.vector(originX, self.yOffset, originZ)
end

function XTile:getRectangle()
	return Rectangle.newXZ(self:getLeftBottomVector(), XTile.tileWidth, XTile.tileHeight)
end

function XTile:getCenterVector()
	local leftBottom = self:getLeftBottomVector()
	return leftBottom + math3d.vector(XTile.tileWidth/2, 0, XTile.tileHeight/2)
end

function XTile:getArea()
	return XTile.tileWidth * XTile.tileHeight
end

return XTile