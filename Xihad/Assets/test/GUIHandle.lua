local function findWindow(name, parentWindow)
	parentWindow = parentWindow or CEGUI.System:getSingleton():getDefaultGUIContext():getRootWindow()
	return parentWindow:getChild(name)
end

local function vector2fToUVector(vector2f)
	local uvector = CEGUI.UVector2:new()
	uvector.x.scale = 0
	uvector.x.offset = vector2f.x
	uvector.y.scale = 0
	uvector.y.offset = vector2f.y
	return uvector
end

local function followMouse(window)
	local position = CEGUI.System:getSingleton():getDefaultGUIContext():getMouseCursor():getPosition()
	window:setPosition(vector2fToUVector(position))
end

local function addMenuItems(popupMenu, list)
	for k,v in pairs(list) do
		local item = CEGUI.WindowManager:getSingleton():createWindow("Xihad/MenuItem")
		item:setText(k)
		if not v then item:setDisabled(true) end
		popupMenu:addItem(CEGUI.toMenuItem(item))
	end
end

--------------------- ShowWindow Implementation ---------------------
--[[
	CommandWindow 	
		- args : 列表项， 默认参数有 shortcut = nil, list = nil, disabled = false
			{ 
				["技能"] = { shortcut = "A", list = { ["技能1"] = true , ["技能2"] = false}) }, 
				["道具"] = { disabled = true }, 
				["待机"] = {} 
			}
--]]
local function showCommandWindow(args)
	local command = CEGUI.toPopupMenu(findWindow("CommandWindow"))
	for i = 1, command:getItemCount() do
		local item = CEGUI.toMenuItem(command:getItemFromIndex(i - 1))
		local name = item:getText()
		local popupList = item:getPopupMenu()
		if popupList then popupList:resetList() end

		local option = args[name]
		if option then
			if option.disabled then item:setDisabled(true) end
			if option.list then addMenuItems(popupList, option.list) end
			-- TOTO shortcut
		end
	end
	
	followMouse(command)
	command:openPopupMenu()
	return command
end
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

local GUIHandle = {
	ShowWindowImpl = { 
		CommandWindow = showCommandWindow,
		AttackDamage = showAttackDamageLabel,
		ForTest = function (args)
			findWindow("Test"):setText(args)
		end
	},
	HideWindowImpl = {
		CommandWindow = function ()
			CEGUI.toPopupMenu(findWindow("CommandWindow")):closePopupMenu()
		end
		
	}
}

function GUIHandle:subscribeEvent(eventName, callback)
	local subscribeEventImpl = function (window, eventName, functorName)
		if not window:isEventPresent(eventName) then
			window:subscribeEvent(eventName, functorName)
		end
	end
	
	if eventName == "CommandSelected" then
		local commandWindow = findWindow("CommandWindow")
		CommandSelected__ = function  (args)
			local selected = commandWindow:getFirstSelectedItem()
			if not selected:isDisabled() then
				callback()
			end
		end
		subscribeEventImpl(commandWindow, "ListSelectionAccepted", "CommandSelected__")
	end

end

--[[------------------------------------------------------------------------------
	CommandWindow 	
		- args : 列表项， 默认参数有 shortcut = nil, list = nil, disabled = false
			{ 	"技能", "道具", "待机",
				["技能"] = { shortcut = "A", list = { "技能1" , "技能2" }) }, 
				["道具"] = { disabled = true }, 
				["待机"] = {} 
			}
	StatusWindow
		- args : { "HP"=?, "MP"=?, "名字"=?, "职业"=?, "技能"={1,2,3}, }
	AttackDamage	
		- args : { damage = 20, position = { x = 300, y = 200 } }
--]]-------------------------------------------------------------------------------

function GUIHandle:showWindow(name, args)
	local showWindowImpl = self.ShowWindowImpl[name]
	return showWindowImpl(args)
end

function GUIHandle:hideWindow(name)
	local hideWindowImpl = self.HideWindowImpl[name]
	if hideWindowImpl then hideWindowImpl() end
end

--------------------- EventHandler --------------------------

return GUIHandle