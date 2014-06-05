local Utils = require "ui.StaticUtils"
local ParcelView = require "ui.ParcelView"

local Notification = {
	completeListeners = {},
	cancelListeners = {},
	window = Utils.findWindow("Notification", nil, true)
}

local function getColorTag(color)
	return string.format("[colour='%s']", color)
end

function Notification:_formatString(fmt, valueDict, colorDict)
	local defaultColor = getColorTag("FF412502")
	if not valueDict then 
		return defaultColor..fmt 
	end
	
	return defaultColor..string.gsub(fmt, "@(%w+)", function (matched)
		local value = valueDict[matched]
		
		if not value then
			return "@"..matched 
		end
			
		if colorDict and colorDict[matched] then
			value = getColorTag(colorDict[matched])..value..defaultColor
		end
		
		return value
	end)
end

function Notification:_replaceString(fmt, valueDict)
	return (string.gsub(fmt, "@(%w+)", valueDict or {}))
end

function Notification:_updateText(fmt, valueDict, colorDict)
	local font = self.window:getFont()
	local extent = font:getTextExtent(self:_replaceString(fmt, valueDict))
	local line = font:getFontHeight()
	local margin = self.window:getProperty("TextMargin")
	
	local width = Utils.newUDim(0, extent + 2*margin)
	local height = Utils.newUDim(0, line + 2*margin)
	local parent = self.window:getParent()
	parent:setSize(CEGUI.USize:new(width, height))
	
	self.window:setText(self:_formatString(fmt, valueDict, colorDict))
end

function Notification:show(fmt, valueDict, colorDict)
	assert(type(fmt) == "string")
	self:_updateText(fmt, valueDict, colorDict)
	Utils.fireEvent("Unfold", self.window)
	return self.window:getParent()
end

function Notification:close()
	Utils.fireEvent("Fold", self.window)
end

function Notification:addListener(listener, eventType)
	print("I have no listener")
end

function Notification:removeListener(listener, eventType)
	print("I have no listener")
end

return Notification