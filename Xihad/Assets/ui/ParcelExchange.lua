local Utils = require "ui.StaticUtils"
local ParcelView = require "ui.ParcelView"

local ParcelExchange = {
	completeListeners = {},
	cancelListeners = {},
	master = ParcelView.new(
		CEGUI.toItemListbox(Utils.findWindow("ParcelExchange/MasterSlots")),
		Utils.findWindow("ParcelExchange/MasterName"),
		Utils.findWindow("ParcelExchange/L2RArrow"),
		0.22),
	guest = ParcelView.new(
		CEGUI.toItemListbox(Utils.findWindow("ParcelExchange/GuestSlots")),
		Utils.findWindow("ParcelExchange/GuestName"),
		Utils.findWindow("ParcelExchange/R2LArrow"),
		0.78),
		
	FrameWindow = Utils.findWindow("ParcelExchange"),
	_model = nil,
}

function ParcelExchange:show(model)
	self._model = model
	local master, guest = self.master, self.guest
	master:setModel(model:getMaster())
	guest:setModel(model:getGuest())
	
	local adjustedWidth = math.max(master.listbox:getPixelSize().width,
		guest.listbox:getPixelSize().width)
	master:relayout(adjustedWidth)
	guest:relayout(adjustedWidth)
	
	return self.FrameWindow
end

function ParcelExchange:close()
	
end

function ParcelExchange:onItemClick(e)
	local itemEntry = CEGUI.toMenuItem(CEGUI.toWindowEventArgs(e).window)
	
	self.master:onItemClick(itemEntry, self.guest)
	self.guest:onItemClick(itemEntry, self.master)
end

function ParcelExchange:onItemHover(e)
	self.master:onItemHover(e)
	self.guest:onItemHover(e)
end

function ParcelExchange:onItemHoverNil(e)
	if not CEGUI then
		return 
	end
	
	self.master:onItemHoverNil(e)
	self.guest:onItemHoverNil(e)
end

function ParcelExchange:onComplete(e)
	for callback,_ in pairs(self.completeListeners) do
		callback(self._model)
	end
end

function ParcelExchange:onTidy(e)
	self.master:tidyParcel()
	self.guest:tidyParcel()
end

function ParcelExchange:_changeListeners(listener, eventType, setting)
	if eventType == "Complete" then
		self.completeListeners[listener] = setting
	elseif eventType == "Cancel" then
		self.cancelListeners[listener] = setting
	else
		return { "Complete", "Cancel" }
	end
end

function ParcelExchange:addListener(listener, eventType)
	return self:_changeListeners(listener, eventType, true)
end

function ParcelExchange:removeListener(listener, eventType)
	return self:_changeListeners(listener, eventType, nil)
end

local function initWidget()
	local window = Utils.findWindow("ParcelExchange")
	
	local bt1 = window:getChild("Tidy")
	local bt2 = window:getChild("Complete")
	local btSize = bt1:getPixelSize()
	local btY = Utils.newUDim(1, -1.5*btSize.height)
	bt1:setYPosition(btY)
	bt2:setYPosition(btY)
	bt1:setXPosition(Utils.newUDim(0.3, -0.5*btSize.width))
	bt2:setXPosition(Utils.newUDim(0.7, -0.5*btSize.width))
	g_scheduler:runOnMainThread(function()
			bt1:subscribeEvent("Clicked", 
				G_CEGUISubscriberSlot.ExchangeTidy)
			bt2:subscribeEvent("Clicked", 
				G_CEGUISubscriberSlot.ExchangeComplete)
		end)

	ParcelExchange.master:init("green", -1)
	ParcelExchange.guest:init("red", 0)	
	
	-- exchangeWindow
	local screenSz = window:getParentPixelSize()
	local wndSz = window:getPixelSize()
	window:setXPosition(Utils.newUDim(0.5, -wndSz.width*0.5))
	window:setYPosition(Utils.newUDim(0.5, -wndSz.height*0.5))
end

initWidget()

return ParcelExchange