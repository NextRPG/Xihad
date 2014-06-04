local Utils = require "ui.StaticUtils"
local ParcelView = require "ui.ParcelView"

local parcelWindow = Utils.findWindow("ParcelPanel/ParcelExchange")

local ParcelExchange = {
	completeListeners = {},
	cancelListeners = {},
	master = ParcelView.new(
		CEGUI.toItemListbox(Utils.findWindow("MasterSlots", parcelWindow)),
		Utils.findWindow("MasterName", parcelWindow),
		Utils.findWindow("L2RArrow", parcelWindow),
		0.22),
	guest = ParcelView.new(
		CEGUI.toItemListbox(Utils.findWindow("GuestSlots", parcelWindow)),
		Utils.findWindow("GuestName", parcelWindow),
		Utils.findWindow("R2LArrow", parcelWindow),
		0.78),
		
	FrameWindow = parcelWindow,
	_model = nil,
}

function ParcelExchange:show(model)
	self._model = model
	local master, guest = self.master, self.guest
	master:setModel(model.masterName, model.masterParcel)
	guest:setModel(model.guestName, model.guestParcel)
	
	local adjustedWidth = math.max(master.listbox:getPixelSize().width,
		guest.listbox:getPixelSize().width)
	master:relayout(adjustedWidth)
	guest:relayout(adjustedWidth)

	self.FrameWindow:setProperty("Size", "{{0, 0}, {0, 0}}")
	self.FrameWindow:setAlpha(1)
	self.FrameWindow:setVisible(true)
	Utils.fireEvent("PullDown", self.FrameWindow)
	return self.FrameWindow:getParent()
end

function ParcelExchange:close()
	Utils.fireEvent("FrameHide", self.FrameWindow)
	self.master:reset()
	self.guest:reset()
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
		callback(self._model, "Complete")
	end
end

function ParcelExchange:onTidy(e)
	self.master:tidyParcel()
	self.guest:tidyParcel()
end

function ParcelExchange:onCancel(e)
	for callback,_ in ipairs(self.cancelListeners) do
		callback("Cancel")
	end
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
	local window = parcelWindow
	local tidyBtn = window:getChild("Tidy")
	local okBtn = window:getChild("Complete")
	local btSize = tidyBtn:getPixelSize()
	local btY = Utils.newUDim(1, -1.5*btSize.height)
	tidyBtn:setYPosition(btY)
	okBtn:setYPosition(btY)
	tidyBtn:setXPosition(Utils.newUDim(0.3, -0.5*btSize.width))
	okBtn:setXPosition(Utils.newUDim(0.7, -0.5*btSize.width))
	
	local closeBtn = window:getChild("Close")
	closeBtn:setXPosition(Utils.newUDim(1,-8-closeBtn:getPixelSize().width))
	closeBtn:setYPosition(Utils.newUDim(0, 8))
	
	g_scheduler:runOnMainThread(function()
			tidyBtn:subscribeEvent("Clicked", 
				G_CEGUISubscriberSlot.ExchangeTidy)
			okBtn:subscribeEvent("Clicked", 
				G_CEGUISubscriberSlot.ExchangeComplete)
			closeBtn:subscribeEvent("Clicked",
				G_CEGUISubscriberSlot.ExchangeCancel)
		end)
	
	ParcelExchange.master:init("green", -1)
	ParcelExchange.guest:init("red", 0)	
end

initWidget()

return ParcelExchange