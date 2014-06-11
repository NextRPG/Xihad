local StaticUtils = {}

function StaticUtils.findWindow(name, parentWindow, recursive)
	parentWindow = parentWindow or CEGUI.System:getSingleton():getDefaultGUIContext():getRootWindow()
	if recursive then 
		return parentWindow:getChildRecursive(name)
	end
	
	return parentWindow:getChild(name)
end

function StaticUtils.vector2fToUVector(vector2f)
	local uvector = CEGUI.UVector2:new()
	uvector.x.scale = 0
	uvector.x.offset = vector2f.x
	uvector.y.scale = 0
	uvector.y.offset = vector2f.y
	return uvector
end

function StaticUtils.followMouse(window)
	local position = CEGUI.System:getSingleton():getDefaultGUIContext():getMouseCursor():getPosition()
	window:setPosition(StaticUtils.vector2fToUVector(position))
end

function StaticUtils.newUDim(scale, offset)
	local udim = CEGUI.UDim:new()
	udim.scale = scale or 0
	udim.offset = offset or 0
	return udim
end

function StaticUtils.fireEvent(eventName, window)
	window:fireEvent(eventName, CEGUI.WindowEventArgs:new(window))
end

function StaticUtils.sizeToWrapText(window, text)
	local text = text or window:getText()

	local font
	if window:getProperty("Font") ~= "" then
		font = CEGUI.FontManager:getSingleton():get(window:getProperty("Font"))
	else
		font = window:getGUIContext():getDefaultFont()
	end
	
	local extent = font:getTextExtent(text)
	local line = font:getFontHeight()
	local margin = window:getProperty("TextMargin")
	
	local width = StaticUtils.newUDim(0, extent + 2*margin)
	local height = StaticUtils.newUDim(0, line + 2*margin)
	return CEGUI.USize:new(width, height)
end

function StaticUtils.subscribeEvent(target, eventName, func)
	g_scheduler:runOnMainThread(function()
		target:subscribeEvent(eventName, func)
	end)
end

return StaticUtils