local GUIController = {}

G_CEGUISubscriberSlot = {
	CommandSelect = function (e) GUIController.Command:onSelect(e) end,
	CommandHover = function (e) GUIController.Command:onHover(e) end,
	CommandHoverNil = function (e) GUIController.Command:onHoverNil(e) end,
	
	ExchangeItemClick = function (e) GUIController.ParcelExchange:onItemClick(e) end,
	ExchangeItemHover = function (e) GUIController.ParcelExchange:onItemHover(e) end,
	ExchangeItemHoverNil = function (e) GUIController.ParcelExchange:onItemHoverNil(e) end,
	ExchangeComplete = function (e) GUIController.ParcelExchange:onComplete(e) end,
	ExchangeTidy = function (e) GUIController.ParcelExchange:onTidy(e) end,
	ExchangeCancel = function (e) GUIController.ParcelExchange:onCancel(e) end,
}

function GUIController:init()
	self.Command = require "ui.Command"
	self.MapTileInfo = require "ui.MapTileInfo"
	self.ParcelExchange = require "ui.ParcelExchange"
	self.Notification = require "ui.Notification"
	self.FightStart = require "ui.FightStart"
	self.GainExp = require "ui.Progress"
	
	for k,v in pairs(self) do
		if type(v) == "table" and v.init then
			v:init()
		end
	end
	
	g_scheduler:runOnMainThread(function ()
		local Utils = require "ui.StaticUtils"
		local parent = Utils.findWindow("GainExpPanel")
		local window = CEGUI.toProgressBar(parent:getChild("GainExp"))
		
		window:subscribeEvent("AnimationEnded", 
			function (e)
				self.GainExp:onAnimationEnded(e)
			end)
	end)
end

return GUIController