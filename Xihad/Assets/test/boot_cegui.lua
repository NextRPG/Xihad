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

-- load animation
require "assets.ui.Animations"

-- ont only for test but an example for you.
local controller = require("assets.ui.GUIController")
g_scene:pushController({
	onKeyDown = function (self, e, param)
		local handled = 0
		if e.key == "C" then

			controller:showWindow("Command", 
			{ 	
				[1] = { name = "技能", 
						hover = true,
						list = {[1] = {name = "狱火", value = 3 }, 
								[2] = {name = "破冰刃", value =12, disabled = true },
								[3] = {name = "普通", }
								},
					  } ,
				[2] = { name = "道具", list = { [1] = { name = "药", value = 10}}},
				[3] = { name = "交换", disabled = true, },
				[4] = { name = "待机", },
			})
		elseif e.key == "M" then
 			controller:showWindow("MapTileInfo",
 			{
 				name = "草地",
				effects = {
					[1] = { attrName = "攻击力", effectLevel = 3 },
					[2] = { attrName = "攻击力", effectLevel = 2 },
					[3] = { attrName = "攻击力", effectLevel = 1 }
				}
 			})
		elseif e.key == "Q" then
			controller:hideWindow("Command")
			controller:hideWindow("MapTileInfo")
		elseif e.key == "I" then
			-- damageNumber = damageNumber or 0
			-- controller:showWindow("AttackDamage", { damage = damageNumber})
			-- damageNumber = (damageNumber + 5)%1000	
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
controller:subscribeEvent("Command.Select", function (parent, child, eventType)
				print(parent, child, eventType)
			end)
controller:subscribeEvent("Command.Hover", function (parent, child, eventType)
				print(parent, child, eventType)
			end)