local Utils = require "ui.StaticUtils"
local ParcelView = require "ui.ParcelView"

local Notification = {
	completeListeners = {},
	cancelListeners = {},
	window = nil,
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

function Notification:_update(fmt, valueDict, colorDict)
	local sz = Utils.sizeToWrapText(
		self.window, self:_replaceString(fmt, valueDict))
	
	local mouseIcon = self.window:getChild("Mouse")
	local iconSz = mouseIcon:getPixelSize()
	mouseIcon:setXPosition(Utils.newUDim(0, sz.width.offset - iconSz.width - 20))
	mouseIcon:setYPosition(Utils.newUDim(0, sz.height.offset - iconSz.height - 10))
	self.window:getParent():setSize(sz)
	self.window:setText(self:_formatString(fmt, valueDict, colorDict))
end

function Notification:show(fmt, valueDict, colorDict)
	assert(type(fmt) == "string")
	self:_update(fmt, valueDict, colorDict)
	Utils.fireEvent("_Open", self.window)
	return self.window:getParent()
end

function Notification:close()
	Utils.fireEvent("_Close", self.window)
end

function Notification:init()
	self.window = Utils.findWindow("Notification", nil, true)
end

return Notification