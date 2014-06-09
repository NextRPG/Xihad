local Window = require 'GUI.Window'
local TileInfoWindow = require 'ui.MapTileInfo'
local TileInfoView = {
	viewX = 10,
	viewY = 10,
}

function TileInfoView.show(tile)
	local tileWindowArg = {}
	tileWindowArg.name = tile:getTerrain():getName()
	tileWindowArg.effects = {}
	-- { attrName = "攻击力", effectLevel = getRandomEffectValue() },
	
	local wnd = TileInfoWindow:show(tileWindowArg)
	Window.setPosition(wnd, TileInfoView.viewX, TileInfoView.viewY)
end

function TileInfoView.hide()
	TileInfoWindow:close()
end

return TileInfoView
