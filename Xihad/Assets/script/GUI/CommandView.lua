local Window = require 'GUI.Window'
local functional   = require 'std.functional'
local XihadTileSet = require 'Chessboard.XihadTileSet'
local GUIController= require 'ui.GUIController'
local CommandView = {
	skillEntry= '技能',
	itemEntry = '道具',
	swapEntry = '交换',
	stdbyEntry= '待机',
}

function CommandView._addEntry(viewData, name, hover, disabled)
	local entry = {
		name = CommandView[name],
		hover = hover or false,
		disabled = disabled or false,
	}
	
	table.insert(viewData, entry)
	return entry
end

function CommandView._createSkillList(skillCaster)
	local list = {}
	for skill, rest, total in skillCaster:allSkills() do
		-- TODO show total?
		table.insert(list, {
				name = skill:getName(),
				value= string.format('%2s', rest),
				disabled = not skillCaster:canCast(skill),
			})
	end
	
	return list
end

function CommandView._createItemList(warrior)
	local parcel = warrior:findPeer(c'Parcel')
	local list = {}

	for index, item, count in parcel:allSlots() do
		table.insert(list, {
				name = item:getName(),
				value= parcel:getValue(index),
				disabled = not item:canUse(warrior),
			})
	end
	
	return list
end

function CommandView.hook(playStateMachine)
	local listener = functional.bindself(playStateMachine, 'onUICommand')
	CommandView.setHoverListener(listener)
	CommandView.setSelectListener(listener)
	
	playStateMachine:addStateListener('ChooseCommand', {
			onStateEnter = function() 
				local cmdList = playStateMachine:getCommandList()
				local w, h = g_window:getScreenSize()
				CommandView.show(cmdList:getSource(), w/2+50, h/2-80)
			end,
			
			onStateExit = function ()
				CommandView.close()
			end
		})
end

function CommandView.setHoverListener(hoverListener)
	GUIController:subscribeEvent('Command.Hover', hoverListener)
end
	
function CommandView.setSelectListener(selectListener)
	GUIController:subscribeEvent('Command.Select', selectListener)
end

function CommandView.setList(entry, list)
	if #list == 0 then
		entry.disabled = true
	else
		entry.list = list
	end
end

function CommandView.canExchange(warrior)
	local set = XihadTileSet.create(warrior:exchangables())
	return next(set) ~= nil
end

function CommandView.show(warrior, x, y)
	local viewData = {}
	
	local skillEntry= CommandView._addEntry(viewData, 'skillEntry', true)
	local skillList = CommandView._createSkillList(warrior:findPeer(c'SkillCaster'))
	CommandView.setList(skillEntry, skillList)
	
	local itemEntry= CommandView._addEntry(viewData, 'itemEntry')
	local itemList = CommandView._createItemList(warrior)
	CommandView.setList(itemEntry, itemList)
	
	local canExchange = CommandView.canExchange(warrior)
	CommandView._addEntry(viewData, 'swapEntry', nil, not canExchange)
	CommandView._addEntry(viewData, 'stdbyEntry')
	
	local wnd = GUIController:showWindow('Command', viewData)
	Window.setPosition(wnd, x, y)
	return wnd
end

function CommandView.close()
	GUIController:hideWindow('Command')
end

return CommandView
