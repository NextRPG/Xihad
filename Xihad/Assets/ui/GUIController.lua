local GUIController = {
	Command = require "assets.ui.Command",
	MapTileInfo = require "assets.ui.MapTileInfo",
	
}

G_CEGUISubscriberSlot = {
	CommandSelect = function (e)
		GUIController.Command:onEvent(e, "Select")	
	end,
	CommandHover = function (e)
		GUIController.Command:onEvent(e, "Hover")
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
	if eventName == "Command.Select" then
		self.Command:pushSelectListener(callback)
	elseif eventName == "Command.Hover" then
		self.Command:pushHoverListener(callback)	
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