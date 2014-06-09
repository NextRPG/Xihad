local StaticUtils = require 'ui.StaticUtils'

local Window = {}

function Window.setPosition(wnd, offsetX, offsetY)
	wnd:setProperty("Position", 
		string.format('{{0, %d}, {0, %d}}', offsetX, offsetY))
end

function Window.placeCenter(wnd)
	local wndSz = wnd:getPixelSize()
	wnd:setXPosition(StaticUtils.newUDim(0.5, -wndSz.width*0.5))
	wnd:setYPosition(StaticUtils.newUDim(0.5, -wndSz.height*0.5))
end

return Window
