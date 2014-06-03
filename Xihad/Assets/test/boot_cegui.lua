package.cpath = package.cpath ..";../Debug/?.dll" --.."../Debug/?.dll"
require 'Assets.script.AllPackages'	-- change package.path
require "cegui"
local functional = require 'std.functional'


local TaskScheduler = require 'Scheduler.TaskScheduler'
-- create g_scheduler
g_scheduler = TaskScheduler.new()
g_scene:appendUpdateHandler {
	onUpdate = functional.bindself(g_scheduler, 'onUpdate')
}


g_scene:requireSystem(c'Render')
g_cursor:setVisible(false)
local guiUpdater = createCEGUIUpdateHandler(g_engine:getWindow())
g_scene:appendUpdateHandler(guiUpdater)
local controller = g_scene:pushController({
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
controller:drop()

g_scene:pushController(guiUpdater:getEventReceiver())

local sm = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
schemeMgr:createFromFile("Xihad.scheme")
schemeMgr:createFromFile("TaharezLook.scheme")
	
local root = CEGUI.WindowManager:getSingleton():loadLayoutFromFile("xihad_main.layout")
local context = sm:getDefaultGUIContext()
context:setRootWindow(root)
context:setDefaultFont(CEGUI.FontManager:getSingleton():get("fangzheng-14"))
context:getMouseCursor():setDefaultImage("TaharezLook/MouseArrow")

-- load animation
require "ui.Animations"


-- ont only for test but an example for you.
local BaseItem = require "Item.BaseItem"
local BaseParcel = require "BaseParcel"
local EquipmentItem = require "Item.EquipmentItem"

local GUIController = require("assets.ui.GUIController")
local controller = g_scene:pushController({
	onKeyDown = function (self, e, param)
		local handled = 0
		if e.key == "C" then
			GUIController:showWindow("Command",
				{ 	
				[1] = { name = "技能", 
						hover = true,
						list = {[1] = {name = "技能1", value = "3" }, 
								[2] = {name = "技能2", value = "10", disabled = true }, 
								[3] = {name = "技能3", value = "E" }, 
								},
					  } ,
				[2] = { name = "道具", },
				[3] = { name = "交换", disabled = true, },
				[4] = { name = "待机", },
			})
		elseif e.key == "M" then
			local function getRandomEffectValue()
				local result = math.random(-3, 3)
				return result
			end
 			GUIController:showWindow("MapTileInfo",
 			{
 				name = "草地",
				effects = {
					[1] = { attrName = "攻击力", effectLevel = getRandomEffectValue() },
					[2] = { attrName = "攻击力", effectLevel = getRandomEffectValue() },
					[3] = { attrName = "攻击力", effectLevel = getRandomEffectValue() },
				}
 			})
		elseif e.key == "Q" then
			GUIController:hideWindow("Command")
			GUIController:hideWindow("MapTileInfo")
		elseif e.key == "I" then
			local aItem = BaseItem.new("伤药", "RedNumber/-", nil, 5)
			local bItem = BaseItem.new("解毒药", "RedNumber/1", nil, 3)
			local cItem = EquipmentItem.new("大锤", "RedNumber/2")
			local dItem = EquipmentItem.new("长矛", "RedNumber/2")
			local eItem = BaseItem.new("陷阱", "RedNumber/3", nil, 2)
			
			local pL = BaseParcel.new()
			function pL:getTotalSlotCount() return 5 end
			function pL:getValue(index)
				return string.format('%2d', self:getCountAt(index))
			end
			pL:gainItem(aItem, 6)
			pL:gainItem(cItem, 1)
			pL:gainItem(eItem, 1)
			
			local pR = BaseParcel.new()
			function pR:getTotalSlotCount() return 5 end
			pR:gainItem(dItem, 1)
			pR:gainItem(aItem, 3)
			pR:gainItem(bItem, 7)
			function pR:getValue(index)
				return string.format('%2d', self:getCountAt(index))
			end

			local model = {}
			function model:getMaster() 
				local warrior = {}
				function warrior:getName()
					return "路飞"
				end
				function warrior:getParcel()
					return pL
				end
				return warrior
			end
			
			function model:getGuest()
				local warrior = {}
				function warrior:getName()
					return "索隆"
				end
				function warrior:getParcel()
					return pR
				end
				return warrior
			end
			
			
			GUIController:showWindow("ParcelExchange", model)
			-- damageNumber = damageNumber or 0
			-- GUIController:showWindow("AttackDamage", { damage = damageNumber})
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
controller:drop()

local function hoverListener(parent, child, eventType)
	print(parent, child, eventType)
end

local function selectListener(parent, child, eventType)
	GUIController:unsubscribeEvent("Command.Hover", hoverListener)
	print(parent, child, eventType)
end

GUIController:subscribeEvent("Command.Select", selectListener)
GUIController:subscribeEvent("Command.Hover", hoverListener)

