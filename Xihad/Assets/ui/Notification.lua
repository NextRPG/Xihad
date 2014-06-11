local Utils = require "ui.StaticUtils"

local Notification = {
	completeListeners = {},
	cancelListeners = {},
	window = nil,
	MinLength = 200,
	Padding = 30,
	IconOffsetX = -20,
	IconOffsetY = -10,
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

function Notification:_update(fmt, valueDict, colorDict, iconHide)
	local sz = Utils.sizeToWrapText(
		self.window, self:_replaceString(fmt, valueDict))
	sz.width.offset = sz.width.offset + self.Padding
	sz.width.offset = math.max(sz.width.offset, self.MinLength)
	
	
	local mouseIcon = self.window:getChild("Mouse")
	local iconSz = mouseIcon:getPixelSize()
	local pos = CEGUI.UVector2:new(
		Utils.newUDim(0, sz.width.offset - iconSz.width + self.IconOffsetX),
		Utils.newUDim(0, sz.height.offset - iconSz.height + self.IconOffsetY))
	mouseIcon:setPosition(pos)
	mouseIcon:setVisible(not iconHide)
	self.window:getParent():setSize(sz)
	self.window:setText(self:_formatString(fmt, valueDict, colorDict))
end

function Notification:show(fmt, valueDict, colorDict, autoClose)
	assert(type(fmt) == "string")
	self:_update(fmt, valueDict, colorDict, autoClose)
	Utils.fireEvent(autoClose and "_OpenClose" or "_Open", self.window)
	return self.window:getParent()
end

function Notification:close()
	Utils.fireEvent("_Close", self.window)
end

function Notification:init()
	self.window = Utils.findWindow("Notification", nil, true)
end

return Notification