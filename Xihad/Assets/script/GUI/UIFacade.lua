local functional = require 'std.functional'
local TileInfoView= require 'GUI.TileInfoView'
local CommandView = require 'GUI.CommandView'
local TransactionView = require 'GUI.TransactionView'
local GUIController = require 'ui.GUIController'
local UIFacade = {
	commandReceiver = nil,
}
UIFacade.__index = UIFacade

function UIFacade.new()
	return setmetatable({}, UIFacade)
end

function UIFacade:setCommandReceiver(receiver)
	local listener = functional.bindself(receiver, 'onUICommand')
	CommandView.hook(listener)
	TransactionView.hook(listener)
end

function UIFacade:showWarriorInfo(warrior)
	print('ui info: ', warrior:getHostObject():getID())
end

function UIFacade:showTileInfo(tile)
	if tile then
		print(tostring(tile:getLocation()))
		TileInfoView.show(tile)
	else
		TileInfoView.hide()
	end
end

function UIFacade:showWarning(msg)
	print(msg)
end

function UIFacade:showCommandView(sourceWarrior)
	-- Command View
	CommandView.show(sourceWarrior)
end

function UIFacade:closeCommandView(self)
	CommandView.close()
end

function UIFacade:updateCommandView(sourceWarrior)
	CommandView.show(sourceWarrior)
end

function UIFacade:showTransaction(srcWarrior, dstWarrior)
	TransactionView.show(srcWarrior, dstWarrior)
end

function UIFacade:closeTransaction()
	TransactionView.close()
end

return UIFacade