package.cpath = package.cpath ..";../Debug/?.dll" --.."../Debug/?.dll"
require "cegui"

cursor:setVisible(false)
local guiUpdater = createCEGUIUpdateHandler(engine:getWindow())
scene:appendUpdateHandler(guiUpdater)
scene:pushController(guiUpdater:getEventReceiver())

local sm = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
schemeMgr:createFromFile("Xihad.scheme")
schemeMgr:createFromFile("AlfiskoSkin.scheme")
schemeMgr:createFromFile("TaharezLook.scheme")
schemeMgr:createFromFile("Generic.scheme")
	
local root = CEGUI.WindowManager:getSingleton():loadLayoutFromFile("MySample.layout")
local context = sm:getDefaultGUIContext()
context:setRootWindow(root)
context:setDefaultFont(CEGUI.FontManager:getSingleton():get("simhei-14"))
context:getMouseCursor():setDefaultImage("TaharezLook/MouseArrow")

local animMgr = CEGUI.AnimationManager:getSingleton()
animMgr:loadAnimationsFromXML("Xihad.anims")

local textFadeIn = animMgr:instantiateAnimation(animMgr:getAnimation("TextFadeIn"))
textFadeIn:setTargetWindow(root:getChild("AttackDamageLabel"))
local frameLighter = animMgr:instantiateAnimation(animMgr:getAnimation("FrameLighter"))
frameLighter:setTargetWindow(root:getChild("CharacterAttributeWindow"))
local frameDarker = animMgr:instantiateAnimation(animMgr:getAnimation("FrameDarker"))
frameDarker:setTargetWindow(root:getChild("CharacterAttributeWindow"))
