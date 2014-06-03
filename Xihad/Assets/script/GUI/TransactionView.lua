local Window = require 'GUI.Window'
local GUIController= require 'ui.GUIController'
local TransactionView = {}

function TransactionView.hook(receiver)
	GUIController:subscribeEvent("ParcelExchange.Complete", receiver)
	GUIController:subscribeEvent("ParcelExchange.Cancel", receiver)
end

local function copyParcel(warrior)
	return warrior:findPeer(c'Parcel'):createView()
end

function TransactionView.show(srcWarrior, dstWarrior)
	local model = {
		masterName = srcWarrior:getName(),
		masterParcel = copyParcel(srcWarrior),
		guestName = dstWarrior:getName(),
		guestParcel = copyParcel(dstWarrior),
	}
	
	local wnd = GUIController:showWindow("ParcelExchange", model)
	local screenWidth = g_window:getScreenSize()
	local size= wnd:getPixelSize()
	Window.setPosition(wnd, (screenWidth - size.width) / 2, 50)
end

function TransactionView.close()
	GUIController:hideWindow('ParcelExchange')
end

return TransactionView
