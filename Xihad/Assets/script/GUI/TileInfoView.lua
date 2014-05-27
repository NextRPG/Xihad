local Window = require 'GUI.Window'
local GUIController= require 'ui.GUIController'
local TileInfoView = {
	viewX = 10,
	viewY = 10,
}

function TileInfoView.show(tile)
	local tileWindowArg = {}
	tileWindowArg.name = tile:getTerrain():getName()
	tileWindowArg.effects = {}
	-- { attrName = "攻击力", effectLevel = getRandomEffectValue() },
	
	local wnd = GUIController:showWindow("MapTileInfo", tileWindowArg)
	Window.setPosition(wnd, TileInfoView.viewX, TileInfoView.viewY)
end

function TileInfoView.hide()
	GUIController:hideWindow('MapTileInfo')
end

return TileInfoView
