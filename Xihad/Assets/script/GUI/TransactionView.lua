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
	
	GUIController:showWindow("ParcelExchange", model)
end

function TransactionView.close()
	GUIController:hideWindow('ParcelExchange')
end

return TransactionView
