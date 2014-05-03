local base = require 'Controller.PlayerState'

local ChooseTileState = {
	attackRange = nil,
	lastTile = nil,
}
ChooseTileState.__index = ChooseTileState
setmetatable(ChooseTileState, base)

function ChooseTileState.new(...)
	return setmetatable(base.new(...), ChooseTileState)
end

function ChooseTileState:onBack()
	-- back to ChooseHeroState, clear marked range
	self.painter:clear(self.commandList.reachableHandle)
	self.painter:clear(self.attackRange)
	return 'back'
end

function ChooseTileState:needCDWhenTouch()
	return false
end

function ChooseTileState:needCDWhenHover()
	return false
end

function ChooseTileState:onTileSelected(tile)
	-- show tile info
	local warrior = self.commandList.source
	if tile:canStay(warrior) then
		local handle = self.painter:mark({ tile }, 'Destination')
		local object = warrior:getHostObject()
		local destination = tile:getLocation()
		if not g_chessboard:canReach(warrior, destination) then
			print('can not reach')
			self.painter:clear(handle)
		else
			self.executor:move(object, destination)
			self.painter:clear(self.attackRange)
			self.painter:clear(handle)
			self.painter:clear(self.commandList.reachableHandle)
			return 'next'
		end
	else
		self.ui:warning('not stayable')
	end
end

function ChooseTileState:onTileHovered(tile)
	if self.lastTile ~= tile then
		local warrior = self.commandList.source
		
		if g_chessboard:canReach(warrior, tile:getLocation()) then
			self.ui:showTileInfo(tile)
			
			self.painter:clear(self.attackRange)
			
			local skillCaster = warrior:findPeer(c'SkillCaster')
			local range = skillCaster:getCastableTiles(tile:getLocation())
			self.attackRange = self.painter:mark(range, 'Attack')
		else
			self.painter:clear(self.attackRange)
		end
		
		self.lastTile = tile
	end
end

return ChooseTileState
