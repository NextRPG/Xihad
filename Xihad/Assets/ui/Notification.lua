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
	self.window:getParent():setSize(Utils.sizeToWrapText(
		self.window, self:_replaceString(fmt, valueDict)))
	
	self.window:setText(self:_formatString(fmt, valueDict, colorDict))
end

function Notification:show(fmt, valueDict, colorDict)
	assert(type(fmt) == "string")
	self:_updateText(fmt, valueDict, colorDict)
	Utils.fireEvent("_Open", self.window)
	return self.window:getParent()
end

function Notification:close()
	Utils.fireEvent("_Close", self.window)
end

return Notification