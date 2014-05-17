local StaticUtils = {}

function StaticUtils.findWindow(name, parentWindow)
	parentWindow = parentWindow or CEGUI.System:getSingleton():getDefaultGUIContext():getRootWindow()
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

return StaticUtils