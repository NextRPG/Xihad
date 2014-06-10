local Window = require 'GUI.Window'
local Progress = require 'ui.Progress'
local functional = require 'std.functional'
local Notification = require 'ui.Notification'
local CommandView = require 'GUI.CommandView'
local TileInfoView= require 'GUI.TileInfoView'
local TransactionView = require 'GUI.TransactionView'
local UIFacade = {}
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
		TileInfoView.show(tile)
	else
		TileInfoView.hide()
	end
end

function UIFacade:showConfirmMessage(fmt, valueDict, colorDict)
	Window.placeCenter(Notification:show(fmt, valueDict, colorDict))
	local controller = g_scene:pushController({
			onMouseEvent = function(self, e)
				if e.type == 'lPressed' then
					Notification:close()
					g_scene:popController()
				end
				
				return 0
			end
		})
	controller:drop()
end

function UIFacade:showWarning(msg)
	print(msg)
end

function UIFacade:showCommandView(sourceWarrior)
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

function UIFacade:showExpProgressBar(p1, p2, callback)
	Progress:show(p1, p2, callback)
end

return UIFacade