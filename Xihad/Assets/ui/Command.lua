local Utils = require "ui.StaticUtils"
local findWindow = Utils.findWindow

local function getIcon(name)
	-- TODO
	return "RedNumber/-"
end

local Command = {
	clickListeners = {},
	hoverListeners = {}
}

function Command:_addMenuItems(parent, itemList, receiveHover)
	for _,options in ipairs(itemList) do
		local item = CEGUI.toMenuItem(CEGUI.WindowManager:getSingleton():createWindow("Xihad/MenuItem"))
		
		local text = ""
		if options.name then 
			text = text..options.name 
			item:setProperty("XihadName", options.name)
			item:setProperty("Icon", getIcon(options.name))
		end
		
		if options.value then 
			text = string.format("%s  %s", text, options.value)
		end
		
		item:setProperty("XihadValue", options.value or "")
		item:setText(text)
		
		if options.disabled then item:setDisabled(true) end
		if options.list then 
			local child = CEGUI.toPopupMenu(CEGUI.WindowManager:getSingleton():createWindow("Xihad/PopupMenu"))
			self:_addMenuItems(child, options.list, receiveHover or options.hover)
			item:setPopupMenu(child)
		end
		parent:addItem(item)
		
		local subscribeEvent = Utils.subscribeEvent
		local functional = require "std.functional"
		subscribeEvent(item, "Clicked", functional.bindself(self, "_onSelect"))
		if receiveHover and not options.disabled then 
			subscribeEvent(item, "MouseEntersArea", functional.bindself(self, "_onHover"))
			subscribeEvent(item, "MouseLeavesArea", functional.bindself(self, "_onHoverNil"))
		end
	end
end
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
	self:_addMenuItems(command, args)
		
	command:openPopupMenu()
	return command
end

function Command:close()
	CEGUI.toPopupMenu(findWindow("CommandWindow")):closePopupMenu()
end

local function explainEvent(e)
	local item = CEGUI.toMenuItem(CEGUI.toWindowEventArgs(e).window)
	
	-- escape callback when click high level menu, which has child menu.
	if item:getPopupMenu() or item:isDisabled() then return end	
	
	local name = item:getProperty("XihadName")
	-- local value = tonumber(item:getProperty("XihadValue"))

	local parent = item:getOwnerList():getParent()
	local hasParentMenu = parent:getType() == "Xihad/MenuItem"

	local parentName = hasParentMenu and parent:getProperty("XihadName") or name
	local childName = hasParentMenu and name or nil
	
	return parentName, childName
end

function Command:_onHover(e)
	local parentName, childName = explainEvent(e)
	for callback,_ in pairs(self.hoverListeners) do
		callback(parentName, childName, "Hover")
	end
end

function Command:_onSelect(e)
	local parentName, childName = explainEvent(e)
	for callback,_ in pairs(self.clickListeners) do
		callback(parentName, childName, "Select")
	end
end

function Command:_onHoverNil(e)
	for callback,_ in pairs(self.hoverListeners) do
		callback(nil, nil, "Hover")
	end
end

function Command:_changeListeners(listener, eventType, setting)
	if eventType == "Hover" then
		self.hoverListeners[listener] = setting
	elseif eventType == "Select" then
		self.clickListeners[listener] = setting
	else
		return {"Hover", "Select"}
	end
end

function Command:addListener(listener, eventType)
	return self:_changeListeners(listener, eventType, true)
end

function Command:removeListener(listener, eventType)
	return self:_changeListeners(listener, eventType, nil)
end

return Command
