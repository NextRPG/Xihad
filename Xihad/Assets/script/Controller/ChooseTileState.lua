local base = require 'Controller.PlayerControlState'

local ChooseTileState = { }
ChooseTileState.__index = ChooseTileState
setmetatable(ChooseTileState, base)

function ChooseTileState.new()
	local obj = setmetatable(base.new(), ChooseTileState)
	return obj
end

function ChooseTileState:onBack()
	-- back to ChooseHeroState, clear marked range
	self.ui:clear()
	self.chessboard:clearRange()
	return 'back'
end

function ChooseTileState:onTileSelected(tile)
	-- show tile info
	if tileObject:canStay(self.commandList:getSource()) then
		self.chessboard:highlightTile(tileObject)
		
		local dest = tile:getLocation()
		local src = self.commandList:getSource()
		src:moveToLocation(dest)
		return 'next'
	else
		self.ui:warning('not stayable')
	end
end

function ChooseTileState:onTileHoverd(tile)
	self.ui:showInfo(tile)
	self.chessboard:showAttackableRange(src, tile:getLocation())
end

return ChooseTileState
