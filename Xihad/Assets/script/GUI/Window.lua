local Window = {}

function Window.setPosition(wnd, x, y)
	wnd:setProperty("Position", string.format('{{0, %d}, {0, %d}}', x, y))
end

return Window
