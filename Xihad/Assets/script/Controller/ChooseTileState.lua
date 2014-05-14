local base = require 'Controller.PlayerState'
local ChooseTileState = setmetatable({
	selectedHandle	= nil,
	reachableHandle = nil,
	attackableHandle= nil,
	
	selectedTile 	= nil,
	prevSourceTile 	= nil,
	
	promotingTile = nil,
}, base)
ChooseTileState.__index = ChooseTileState

function ChooseTileState.new(...)
	return setmetatable(base.new(...), ChooseTileState)
end

function ChooseTileState:_getSourceReachables()
	return g_chessboard:getReachableTiles(self:_getSource())
end

function ChooseTileState:onStateEnter(state, prev)
	if prev == 'ChooseCommand' then
		self:_getSourceBarrier():setTile(self.prevSourceTile)
		assert(self.prevSourceTile)
		
	end
	
	self.prevSourceTile = nil
	
	local reachable = self:_getSourceReachables()
	self.reachableHandle = self:_markRange(reachable, 'Reachable')
	self.camera:focus(self:_getSourceObject())
end

function ChooseTileState:onStateExit()
	self:_safeClear('reachableHandle')
	self:_safeClear('attackableHandle')
	self:_safeClear('selectedHandle')
end

function ChooseTileState:onBack()
	self.camera:focus(nil)
	return 'back'
end

function ChooseTileState:needCDWhenTouch()
	return false
end

function ChooseTileState:needCDWhenHover()
	return false
end

function ChooseTileState:_fastenCusor()
	-- local XihadMapTile = require 'Chessboard.XihadMapTile'
	-- local x, y = g_cursor:getPosition()
	-- local ray = g_collision:getRayFromScreenCoord(x, y)
	-- local point = XihadMapTile.intersectsGround(ray)
	-- local fixCursor = { onUpdate = function ()
	-- 	local x,y = g_collision:getScreenCoordFromPosition(point)
	-- 	g_cursor:setPosition(x, y)
	-- end}
	-- self.camera.cameraObject:appendUpdateHandler(fixCursor)
	
	-- fixCursor:stop()
end

function ChooseTileState:_canReachTile(tile)
	return g_chessboard:canReach(self:_getSource(), tile:getLocation())
end

function ChooseTileState:_moveWarrior(destLocation)
	-- move back to warrior location
	local warriorPosition = self:_getSourceTile():getCenterVector()
	self.camera:translateToTarget(warriorPosition)
	
	-- continue to focus host object
	local sourceObject = self:_getSourceObject()
	self.camera:focus(sourceObject)
	self.executor:move(sourceObject, destLocation)
end

function ChooseTileState:_selectTile(tile)
	-- TODO
	-- self:_fastenCusor()
	self:_focusTile(tile)
	assert(not self.selectedHandle)
	self.selectedHandle = self:_markTile(tile, 'Selected')
	self.selectedTile = tile
end

function ChooseTileState:_confirmTile(tile)
	-- double click or confirm click
	self.prevSourceTile = self:_getSourceTile()
	
	local destHandle = self:_markTile(tile, 'Destination')
	self:_moveWarrior(tile:getLocation())
	self:_clearMark(destHandle)
	
	self.commandList:setLocation(tile:getLocation())
end

function ChooseTileState:onTileSelected(tile, times)
	-- show tile info
	self:_updatePromote(tile)
	self:_safeClear('selectedHandle')
	
	if not tile:canStay(self:_getSource()) then
		self.ui:warning('not stayable')
	elseif not self:_canReachTile(tile) then
		self.ui:warning('can not reach')
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
	
	self.attackableHandle = self:_markRange(attackRange, 'Attack')
	self.promotingTile = tile
end

function ChooseTileState:_makePromote(tile)
	self:_showTileInfo(tile)
	self:_showAttackRange(tile)
end

function ChooseTileState:_updatePromote(tile)
	self:_safeClear('attackableHandle')
	self.promotingTile = nil
	
	if self:_canReachTile(tile) then
		self:_makePromote(tile)
	end
end

function ChooseTileState:onTileHovered(tile)
	if self.promotingTile ~= tile then
		self:_updatePromote(tile)
	end
end

return ChooseTileState
