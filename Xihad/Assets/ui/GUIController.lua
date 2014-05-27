local GUIController = {
	Command = require "assets.ui.Command",
	MapTileInfo = require "assets.ui.MapTileInfo",
	ParcelExchange = require "assets.ui.ParcelExchange",
}

G_CEGUISubscriberSlot = {
	CommandSelect = function (e)
		GUIController.Command:onSelect(e)	
	end,
	CommandHover = function (e)
		GUIController.Command:onHover(e)
	end,
	CommandHoverNil = function (e)
		GUIController.Command:onHoverNil(e)
	end	

}

--[[
AttackDamage	
		- args : { damage = 20, position = { x = 300, y = 200 } }

--]]
local function showAttackDamageLabel(args)
	local damage = args.damage
	local hundreds = math.floor(damage%1000/100)
	local tens = math.floor(damage%100/10)
	local units = damage%10

	local prefix = "RedNumber/"
	local one = hundreds > 0 and prefix..hundreds or (tens > 0 and prefix..tens or prefix..units) 
	local two = hundreds > 0 and prefix..tens or (tens > 0 and prefix..units or "")
	local three = hundreds > 0 and prefix..units or ""
	
	local label = findWindow("AttackDamageLabel")
	label:getChild("One"):setProperty("Image", one)
	label:getChild("Two"):setProperty("Image", two)
	label:getChild("Three"):setProperty("Image", three)

	label:setEnabled(true)
	return label
end

--------------------- Exposed Interface ---------------------

function GUIController:subscribeEvent(eventName, callback)
	local idx = string.find(eventName, "%.")
	local wnd = string.sub(eventName, 1, idx - 1)
	if string.find(eventName, "Select") then
		self[wnd]:addSelectListener(callback)
	elseif string.find(eventName, "Hover") then
		self[wnd]:addHoverListener(callback)
	end
end

function GUIController:unsubscribeEvent(eventName, callback)
	local idx = string.find(eventName, "%.")
	local wnd = string.sub(eventName, 1, idx - 1)
	if string.find(eventName, "Select") then
		self[wnd]:removeSelectListener(callback)
	elseif string.find(eventName, "Hover") then
		self[wnd]:removeHoverListener(callback)
	end
end

--[[------------------------------------------------------------------------------
	StatusWindow
		- args : { "HP"=?, "MP"=?, "名字"=?, "职业"=?, "技能"={1,2,3}, }
	AttackDamage	
		- args : { damage = 20, position = { x = 300, y = 200 } }
--]]-------------------------------------------------------------------------------
function GUIController:showWindow(name, args)
	return self[name]:show(args)
end

function GUIController:hideWindow(name)
	return self[name]:close()
end

--------------------- EventHandler --------------------------

return GUIController