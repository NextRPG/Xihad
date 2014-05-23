local Utils = require "assets.ui.StaticUtils"
local findWindow = Utils.findWindow
local followMouse = Utils.followMouse

local function getIcon(name)
	-- TODO
	return "RedNumber/-"
end

local function addMenuItems(parent, itemList, receiveHover)
	for _,options in ipairs(itemList) do
		local item = CEGUI.toMenuItem(CEGUI.WindowManager:getSingleton():createWindow("Xihad/MenuItem"))
		
		local text = ""
		if options.name then 
			text = text..options.name 
			item:setProperty("XihadName", options.name)
			item:setProperty("Icon", getIcon(options.name))
		end
		
		if options.value then 
			text = string.format("%s %2d", text, options.value)
		end
		item:setProperty("XihadValue", options.value or "")
		item:setText(text)
		
		if options.disabled then item:setDisabled(true) end
		if options.list then 
			local child = CEGUI.toPopupMenu(CEGUI.WindowManager:getSingleton():createWindow("Xihad/PopupMenu"))
			addMenuItems(child, options.list, receiveHover or options.hover)
			item:setPopupMenu(child)
		end
		parent:addItem(item)
		item:subscribeEvent("Clicked", G_CEGUISubscriberSlot.CommandSelect)
		if receiveHover then
			item:subscribeEvent("MouseEntersArea", G_CEGUISubscriberSlot.CommandHover)
		end
	end
end

local Command = {
	clickListeners = {},
	hoverListeners = {}
}
--[[------------------------------------------------------------------------
	CommandWindow 	
		+ 可选参数：
			name(string), list(table), disabled(bool), value(number), hover(bool)
			
			{ 	
				[1] = { name = "技能", 
						hover = true,
						list = {[1] = {name = "技能1", value = "3", disabled = true }, 
								[2] = {...},
								},
					  } ,
				[2] = { name = "道具", ...},
				[3] = { name = "交换", disabled = true, ...},
				[4] = { name = "待机", },
			}
--]]------------------------------------------------------------------------
function Command:show(args)
	local command = CEGUI.toPopupMenu(findWindow("CommandWindow"))
	command:resetList()
	addMenuItems(command, args)
		
	followMouse(command)
	command:openPopupMenu()
	
	return command
end

function Command:close()
	CEGUI.toPopupMenu(findWindow("CommandWindow")):closePopupMenu()
end

function Command:onEvent(e, eventType)
	local item = CEGUI.toMenuItem(CEGUI.toWindowEventArgs(e).window)
	
	-- escape callback when click high level menu, which has child menu.
	if item:getPopupMenu() or item:isDisabled() then return end	
	
	local name = item:getProperty("XihadName")
	local value = tonumber(item:getProperty("XihadValue"))

	local parent = item:getOwnerList():getParent()
	local hasParentMenu = parent:getType() == "Xihad/MenuItem"

	local parentName = hasParentMenu and parent:getProperty("XihadName") or name
	local childName = hasParentMenu and name or nil
	
	for callback,_ in pairs(self.clickListeners) do
		callback(parentName, childName, eventType)
	end
end

function Command:addSelectListener(listener)
	self.clickListeners[listener] = true
end

function Command:addHoverListener(listener)
	self.hoverListeners[listener] = true
end

function Command:removeSelectListener(listener)
	self.clickListeners[listener] = nil
end

function Command:removeHoverListener(listener)
	self.hoverListeners[listener] = nil
end
return Command
