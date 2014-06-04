local GUISystem = {}

function GUISystem.init()
	local guiUpdater = createCEGUIUpdateHandler(g_engine:getWindow())
	g_scene:appendUpdateHandler(guiUpdater)
	local eventReceiver = guiUpdater:getEventReceiver()

	local sm = CEGUI.System:getSingleton()
	local schemeMgr = CEGUI.SchemeManager:getSingleton()
	schemeMgr:createFromFile("Xihad.scheme")
	schemeMgr:createFromFile("TaharezLook.scheme")
	schemeMgr:createFromFile("Generic.scheme")
		
	local root = CEGUI.WindowManager:getSingleton():loadLayoutFromFile("xihad_main.layout")
	local context = sm:getDefaultGUIContext()
	context:setRootWindow(root)
	context:setDefaultFont(CEGUI.FontManager:getSingleton():get("fangzheng-14"))
	context:getMouseCursor():setDefaultImage("TaharezLook/MouseArrow")

	-- load animation
	require "ui.Animations"
	
	local GUIController = require 'ui.GUIController'
	GUIController:init()
	
	return eventReceiver
end

return GUISystem
