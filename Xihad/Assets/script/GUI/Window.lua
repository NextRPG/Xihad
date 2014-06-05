local Window = {}

function Window.setPosition(wnd, offsetX, offsetY)
	wnd:setProperty("Position", 
		string.format('{{0, %d}, {0, %d}}', offsetX, offsetY))
end

return Window
