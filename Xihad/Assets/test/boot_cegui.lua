package.cpath = package.cpath ..";../Debug/?.dll" --.."../Debug/?.dll"
require "cegui"

g_scene:requireSystem(c'Render')
g_cursor:setVisible(false)
local guiUpdater = createCEGUIUpdateHandler(g_engine:getWindow())
g_scene:appendUpdateHandler(guiUpdater)
g_scene:pushController({
		onMouseEvent = function(self, event, arg)
			-- print (event.type)
			if event.type == "lDoubleClick" then
				print("Double click, Processed")
				return 0
			elseif event.type == "rPressed" then
				print("right clicked, but I'm not into it")
				return -1
			end
			
			return 1
		end
	})


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


-- animation
local animMgr = CEGUI.AnimationManager:getSingleton()
animMgr:loadAnimationsFromXML("Xihad.anims")

local animations = 	{ 
						TextFadeIn = { "AttackDamageLabel"},
						
						Magnify = { "LeftDialog/TextArea", "RightDialog/TextArea" },
						Shrink = { "LeftDialog/TextArea", "RightDialog/TextArea" },
						
						PortraitLighter = { "LeftDialog/ImageArea", "RightDialog/ImageArea" },
						PortraitDarker = { "LeftDialog/ImageArea", "RightDialog/ImageArea" },

						DialogueClose = { "LeftDialog", "RightDialog" },
					}

for animName, targetList in pairs(animations) do
	local anim = animMgr:getAnimation(animName)
	for _, name in ipairs(targetList) do
		local targetWindow = root:getChild(name)
		animMgr:instantiateAnimation(anim):setTargetWindow(targetWindow)
	end
end


local controller = require("assets.test.GUIController")
g_scene:pushController({
	onKeyDown = function (self, e, param)
		local handled = 0
		if e.key == "J" then

			controller:showWindow("CommandWindow", 
			{ 

				["技能"] = { shortcut = "A", list = { ["技能1"] = true , ["技能2"] = true } }, 
				["道具"] = { list = { ["道具1"] = true} }, 
				["待机"] = { } 
			})

		elseif e.key == "Q" then
			controller:hideWindow("CommandWindow")
		elseif e.key == "I" then
			damageNumber = damageNumber or 0
			controller:showWindow("AttackDamage", { damage = damageNumber})
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
controller:subscribeEvent("CommandSelected", function (args)
				print(args)
			end)
