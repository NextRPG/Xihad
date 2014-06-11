local Window = require 'GUI.Window'
local TransactionView = {}

function TransactionView.hook(receiver)
	local TransactionWindow = require 'ui.ParcelExchange'
	TransactionWindow:addListener(receiver, 'Complete')
	TransactionWindow:addListener(receiver, 'Cancel')
end

local function copyParcel(warrior)
	return warrior:findPeer(c'Parcel'):createView()
end

function TransactionView.show(srcWarrior, dstWarrior)
	local TransactionWindow = require 'ui.ParcelExchange'
	local model = {
		masterName = srcWarrior:getName(),
		masterParcel = copyParcel(srcWarrior),
		guestName = dstWarrior:getName(),
		guestParcel = copyParcel(dstWarrior),
	}
	
	local wnd = TransactionWindow:show(model)
	local screenWidth = g_window:getScreenSize()
	local size= wnd:getPixelSize()
	Window.setPosition(wnd, (screenWidth - size.width) / 2, 50)
end

function TransactionView.close()
	local TransactionWindow = require 'ui.ParcelExchange'
	TransactionWindow:close()
end

return TransactionView
