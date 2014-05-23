local GUISystem = {}

function GUISystem.init()
	local guiUpdater = createCEGUIUpdateHandler(g_engine:getWindow())
	g_scene:appendUpdateHandler(guiUpdater)
	g_scene:pushController(guiUpdater:getEventReceiver())

	local sm = CEGUI.System:getSingleton()
	local schemeMgr = CEGUI.SchemeManager:getSingleton()
	schemeMgr:createFromFile("Xihad.scheme")
	schemeMgr:createFromFile("TaharezLook.scheme")
	schemeMgr:createFromFile("Generic.scheme")
		
	local root = CEGUI.WindowManager:getSingleton():loadLayoutFromFile("xihad_main.layout")
	local context = sm:getDefaultGUIContext()
	context:setRootWindow(root)
	context:setDefaultFont(CEGUI.FontManager:getSingleton():get("simhei-14"))
	context:getMouseCursor():setDefaultImage("TaharezLook/MouseArrow")

	-- load animation
	require "assets.ui.Animations"
end

return GUISystem
