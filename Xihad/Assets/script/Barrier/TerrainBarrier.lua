local Color = require 'Color'
local Class = require 'std.Class'
local Array = require 'std.Array'
local Barrier = require 'Barrier'
local WarriorBarrier = require 'WarriorBarrier'
local TerrainRegistry= require 'TerrainRegistry'
local CompositeEffect= require 'Effect.CompositeEffect'

local TerrainBarrier = {
	type = nil,
	colors = nil,
	effect = nil,
}
TerrainBarrier.__index = TerrainBarrier
TerrainBarrier.__base = 'Barrier'
setmetatable(TerrainBarrier, Barrier)

function TerrainBarrier.new(type, object)
	local o = Barrier.new()
	setmetatable(o, TerrainBarrier)
	assert(type and TerrainRegistry.passable[type])
	o.type = type
	o.colors = {}
	o.effect = CompositeEffect.new() -- TODO shared effect
	
	-- TODO FIX
	for _, mat in object:findComponent(c'Render'):materials() do
		mat:setColorMaterial('none')
		-- mat:setBlend('src.alpha', '1-src.alpha', 'add')
		-- mat:setMaterialType('trans_alphach')
		-- mat:setZWriteEnable(false)
	end
	
	local colorTable = { Color.green, Color.blue, Color.orange }
	table.insert(o.colors, Color.new(colorTable[o.type]))
	return o
end

Class.delegate(TerrainBarrier, 'addEffect', 'effect')

function TerrainBarrier:onStart()
	self:_updateColor()
end

function TerrainBarrier.getOptUniqueKey()
	return 'TerrainBarrier'
end

local function selectResult(field, type, warrior)
	local dedicated = TerrainRegistry[field][type][warrior:getCareer()] 
	
	if dedicated ~= nil then
		return dedicated
	else
		return TerrainRegistry[field][type]['*']
	end
end

function TerrainBarrier:getName()
	return TerrainRegistry.names[self.type]
end

function TerrainBarrier:keepVacancy()
	return true
end

function TerrainBarrier:canPass( warrior )
	return selectResult('passable', self.type, warrior)
end

function TerrainBarrier:canStay( warrior )
	return selectResult('stayable', self.type, warrior)
end

function TerrainBarrier:getActionPointCost( warrior )
	return selectResult('apcost', self.type, warrior)
end

-- function TerrainBarrier:synchronizeTranslate()
-- 	local aabb = self:findPeer(c'Render'):getAABB()
-- 	local _, height, _ = aabb:extent():xyz()
-- 	local center = self:getTile():getCenterVector()
-- 	center:set(nil, -height, nil)
-- 	self:getHostObject():resetTranslate(center)
-- end

-- function TerrainBarrier:setTile( tile ) 
-- 	if not self.tile then
-- 		Barrier.setTile(self, tile)
		
-- 	else
-- 		-- error("Attempt to change the MapTile of a TerrainBarrier")
-- 	end
-- end

function TerrainBarrier:_updateColor()
	local color = Array.getBack(self.colors)
	local render = self:getHostObject():findComponent(c'Render')
	color:setAlpha(200)
	for _, mat in render:materials() do
		mat:setDiffuseColor(color)
	end
end

function TerrainBarrier:pushColor(color)
	table.insert(self.colors, color)
	self:_updateColor()
	return #self.colors
end

function TerrainBarrier:removeColor(index)
	assert(index > 0 and index <= #self.colors, 
		string.format('index: %d, size: %d', index, #self.colors))
	
	if index == #self.colors then
		self:popColor()
	else
		self.colors[index] = false
	end
end

function TerrainBarrier:_trimColors()
	local index = #self.colors
	while index > 0 and not self.colors[index] do
		self.colors[index] = nil
		index = index - 1
	end
	
	assert(index > 0, 'Overmuch pops')
end

function TerrainBarrier:popColor()
	Array.popBack(self.colors)
	self:_trimColors()
	self:_updateColor()
end

function TerrainBarrier:inhabitWith(other, optKey)
	if optKey == WarriorBarrier.getOptUniqueKey() then
		local warrior = other:findPeer(c'Warrior')
		if warrior then
			self.effect:copy():bindSticky(warrior, self)
		end
	end
end

function TerrainBarrier:leaveFrom(other, optKey)
	if optKey == WarriorBarrier.getOptUniqueKey() then
		local warrior = other:findPeer(c'Warrior')
		if warrior then
			warrior:unbindStickyEffects(self)
		end
	end
end

return TerrainBarrier
