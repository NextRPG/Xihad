local Color = require 'Color'
local Array = require 'std.Array'
local Barrier = require 'Barrier'
local TerrainDB  = require 'TerrainDatabase'

local Terrain = {
	type = nil,
	colors = nil,
}
Terrain.__index = Terrain
Terrain.__base = 'Barrier'
setmetatable(Terrain, Barrier)

function Terrain.new(type, object)
	local o = Barrier.new()
	setmetatable(o, Terrain)
	assert(type and TerrainDB.passable[type])
	o.type = type
	o.colors = {}
	
	-- TODO FIX
	local mat = object:findComponent(c'Render'):getMaterial(0)
	mat:setColorMaterial('none')
	-- mat:setBlend('src.alpha', '1-src.alpha', 'add')
	-- mat:setMaterialType('trans_alphach')
	-- mat:setZWriteEnable(false)
	
	return o
end

function Terrain:onStart()
	local colorTable = { Color.green, Color.blue, Color.red }
	self:pushColor(Color.new(colorTable[self.type]))
end

function Terrain.getOptUniqueKey()
	return 'Terrain'
end

local function selectResult(field, type, warrior)
	local dedicated = TerrainDB[field][type][warrior:getCareer()] 
	
	if dedicated ~= nil then
		return dedicated
	else
		return TerrainDB[field][type]['*']
	end
end

function Terrain:keepVacancy()
	return true
end

function Terrain:canPass( warrior )
	return selectResult('passable', self.type, warrior)
end

function Terrain:canStay( warrior )
	return selectResult('stayable', self.type, warrior)
end

function Terrain:getActionPointCost( warrior )
	return selectResult('apcost', self.type, warrior)
end

function Terrain:setTile( tile ) 
	if not self.tile then
		Barrier.setTile(self, tile)
		local aabb = self:findPeer(c'Render'):getAABB()
		local _, height, _ = aabb:extent():xyz()
		local center = tile:getCenterVector()
		center:set(nil, -height, nil)
		self:getHostObject():resetTranslate(center)
	else
		error("Attempt to change the MapTile of a Terrain")
	end
end

function Terrain:_updateColor()
	local color = Array.getBack(self.colors)
	
	local render = self:getHostObject():findComponent(c'Render')
	color:setAlpha(200)
	for _, mat in render:materials() do
		mat:setDiffuseColor(color)
	end
end

function Terrain:pushColor(color)
	table.insert(self.colors, color)
	self:_updateColor()
	return #self.colors
end

function Terrain:removeColor(index)
	local needUpdate = index == #self.colors
	
	self.colors[index] = nil
	
	if needUpdate then
		self:_updateColor()
	end
end

function Terrain:popColor()
	Array.popBack(self.colors)
	self:_updateColor()
end

return Terrain
