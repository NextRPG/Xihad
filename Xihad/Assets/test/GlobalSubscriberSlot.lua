CommandSelected__ = {
	CallbackSet = { }
}

function CommandSelected__.SubscriberSlot(e) 
	local item = CEGUI.toMenuItem(CEGUI.toWindowEventArgs(e).window)
	local text = item:getText()
	for _, callback in pairs(CommandSelected__.CallbackSet) do
		callback(text)
	end
end

-- 