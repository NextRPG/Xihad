local GUIController = {}

G_CEGUISubscriberSlot = {
	CommandSelect = function (e) GUIController.Command:onSelect(e) end,
	CommandHover = function (e) GUIController.Command:onHover(e) end,
	CommandHoverNil = function (e) GUIController.Command:onHoverNil(e) end,
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