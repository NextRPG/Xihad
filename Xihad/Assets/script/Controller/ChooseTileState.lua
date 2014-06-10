local Table= require 'std.Table'
local base = require 'Controller.PlayerState'
local BarrierUtil = require 'Barrier.BarrierUtil'
local ChooseTileState = setmetatable({
	selectedHandle	= nil,
	reachableHandle = nil,
	attackableHandle= nil,
	
	selectedTile 	= nil,
	prevSourceTile 	= nil,
	
	promotingTile = nil,
	reachableTiles= nil,
}, base)
ChooseTileState.__index = ChooseTileState

function ChooseTileState.new(...)
	return setmetatable(base.new(...), ChooseTileState)
end

function ChooseTileState:_getReachables()
	return g_chessboard:getReachableTiles(self:_getSource())
end

function ChooseTileState:_restoreState()
	assert(self.prevSourceTile)
	self:_getSourceBarrier():setTile(self.prevSourceTile)
	BarrierUtil.synchronizeTranslate(self:_getSourceBarrier())
	
	assert(self.prevRotation)
	self:_getSourceObject():resetRotation(self.prevRotation)
end

function ChooseTileState:onStateEnter(state, prev)
	if prev == 'ChooseCommand' then
		self:_restoreState()
		self:_focusObject(self:_getSourceObject())
	else
		self:_fastenCursorWhen(function ()
			self:_focusObject(self:_getSourceObject())
		end)
	end
	
	local reachableArray = self:_getReachables()
	self:_markRange(reachableArray, 'Reachable', 'reachableHandle')
	self.reachableTiles = Table.toSet(reachableArray)
	
	self.prevRotation = nil
	self.prevSourceTile = nil
	self.selectedTile = nil
end

function ChooseTileState:_clear_handle()
	self:_safeClear('reachableHandle')
	self:_safeClear('attackableHandle')
	self:_safeClear('selectedHandle')
end

function ChooseTileState:onStateExit()
	self:_clear_handle()
	self:_showTileInfo(nil)
	self.reachableTiles = nil
end

function ChooseTileState:onBack()
	self:_focusObject(nil)
	return 'back'
end

function ChooseTileState:needCDWhenTouch()
	return false
end

function ChooseTileState:needCDWhenHover()
	return false
end

function ChooseTileState:_canReachTile(tile)
	return self.reachableTiles[tile] == true
end

function ChooseTileState:_moveWarrior(destLocation)
	-- move back to warrior location
	local warriorPosition = self:_getSourceTile():getCenterVector()
	self.camera:translateToTarget(warriorPosition)
	
	-- continue to focus host object
	local sourceObject = self:_getSourceObject()
	self:_focusObject(sourceObject)
	self.executor:move(sourceObject, destLocation)
end

function ChooseTileState:_selectTile(tile)
	assert(not self.selectedHandle)
	self:_markTile(tile, 'Selected', 'selectedHandle')
	self.selectedTile = tile
	
	self:_fastenCursorWhen(function() 
			self:_focusTile(tile)
		end)
end

function ChooseTileState:_confirmTile(tile)
	-- double click or confirm click
	self.prevSourceTile = self:_getSourceTile()
	self.prevRotation = self:_getSourceObject():getRotation()
	self:_clear_handle()
	
	local destHandle = self:_markTile(tile, 'Destination')
	self:_fastenCursorWhen(function ()
		self:_moveWarrior(tile:getLocation())
	end)
	self:_clearMark(destHandle)
	
	self.commandList:setLocation(tile:getLocation())
end

function ChooseTileState:onTileSelected(tile, times)
	-- show tile info
	self:_updatePromote(tile)
	self:_safeClear('selectedHandle')
	
	if not tile:canStay(self:_getSource()) then
		self.ui:showWarning('not stayable')
	elseif not self:_canReachTile(tile) then
		self.ui:showWarning('can not reach')
	elseif self.selectedTile ~= tile and times == 1 then
		self:_selectTile(tile)
	else
		self:_confirmTile(tile)
		return 'next'
	end
end

function ChooseTileState:_showAttackRange(tile)
	local skillCaster = self:_getSourceCaster()
	local attackRange = skillCaster:getCastableTiles(tile:getLocation())
	
	self:_markRange(Table.extractKeys(attackRange), 'Attack', 'attackableHandle')
end

function ChooseTileState:_makePromote(tile)
	self:_showTileInfo(tile)
	self:_showAttackRange(tile)
	self.promotingTile = tile
end

function ChooseTileState:_updatePromote(tile)
	self:_safeClear('attackableHandle')
	
	self.promotingTile = nil
	if self:_canReachTile(tile) then
		self:_makePromote(tile)
	else
		self:_showTileInfo(nil)
	end
end

function ChooseTileState:onTileHovered(tile)
	if self.promotingTile ~= tile then
		self:_updatePromote(tile)
	end
end

return ChooseTileState
