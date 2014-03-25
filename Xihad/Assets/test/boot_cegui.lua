package.cpath = package.cpath ..";../Debug/?.dll" --.."../Debug/?.dll"
require "cegui"
local uiHandle = require("assets.test.GUIHandle")

cursor:setVisible(false)
local guiUpdater = createCEGUIUpdateHandler(engine:getWindow())
scene:appendUpdateHandler(guiUpdater)
scene:pushController(guiUpdater:getEventReceiver())
scene:pushController({
	onKeyDown = function (self, e, param)
		local handled = 0
		if e.key == "P" then
			uiHandle:subscribeEvent("CommandSelected", function (args)
				print("!!!")
			end)
		elseif e.key == "J" then

			uiHandle:showWindow("CommandWindow", 
			{ 

				["技能"] = { shortcut = "A", list = { ["技能1"] = true , ["技能2"] = false } }, 
				["道具"] = { list = { ["道具1"] = true} }, 
				["待机"] = { disabled = true } 
			})

		elseif e.key == "Q" then
			uiHandle:hideWindow("CommandWindow")
		elseif e.key == "I" then
			damageNumber = damageNumber or 0
			uiHandle:showWindow("AttackDamage", { damage = damageNumber})
			damageNumber = (damageNumber + 5)%1000	
		else
			handled = 1
		end

	 	return handled
	end 
	,
	onKeyUp = function (self, e, param)
		return 1
	end
	,
	onMouseEvent = function (self, e, param)
		return 1
	end
})

local sm = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
schemeMgr:createFromFile("Xihad.scheme")
schemeMgr:createFromFile("AlfiskoSkin.scheme")
schemeMgr:createFromFile("TaharezLook.scheme")
schemeMgr:createFromFile("Generic.scheme")
	
local root = CEGUI.WindowManager:getSingleton():loadLayoutFromFile("xihad_main.layout")
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


-- dialog open and close anim
local leftDialogText = root:getChild("LeftDialog"):getChild("TextArea")
local rightDialogText = root:getChild("RightDialog"):getChild("TextArea")
local jumpin = animMgr:getAnimation("JumpIn")
local jumpout = animMgr:getAnimation("JumpOut")
animMgr:instantiateAnimation(jumpin):setTargetWindow(leftDialogText)
animMgr:instantiateAnimation(jumpin):setTargetWindow(rightDialogText)
animMgr:instantiateAnimation(jumpout):setTargetWindow(leftDialogText)
animMgr:instantiateAnimation(jumpout):setTargetWindow(rightDialogText)
