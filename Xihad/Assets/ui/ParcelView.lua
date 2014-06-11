local Utils = require "ui.StaticUtils"
local ParcelView = { }
local subscribeEvent = Utils.subscribeEvent
ParcelView.__index = ParcelView

function ParcelView.new(listbox, label, arrow, scaleX)
	assert(listbox and label and arrow and scaleX)
	local obj = setmetatable({ }, ParcelView)
	obj.listbox = listbox
	obj.label = label
	obj.arrow = arrow
	obj.scaleX = scaleX
	return obj
end

function ParcelView:_setItemEntryProperty(itemView, name, value, icon)
	itemView:setProperty("XihadName", name or "")
	itemView:setProperty("XihadValue", value or "")
	itemView:setProperty("Icon", icon or "")
	itemView:setText(string.format("%s %s", name or "", value or ""))
	itemView:setDisabled(not name)

	if not name and CEGUI.toMenuItem(itemView):isHovering() then
		self:_stopArrowAnim(itemView)
	end
end

function ParcelView:_notifyDataChange()
	local freeSlot = self.parcel and self.parcel:getFreeSlotCount() or 5
	assert(freeSlot <= 5)
	while freeSlot > 0 do
		local itemView = self.listbox:getItemFromIndex(5-freeSlot)
		self:_setItemEntryProperty(itemView)
		freeSlot = freeSlot - 1
	end
	
	if not self.parcel then 
		return
	end
	
	for index, item, count in self.parcel:allSlots() do
		if index > 5 then break end
		local itemView = self.listbox:getItemFromIndex(index - 1)
		self:_setItemEntryProperty(itemView, item:getName(), 
			self.parcel:getValue(index), item:getIcon())
	end
end

function ParcelView:_startArrowAnim(itemEntry)
	local baseY = self.listbox:getYPosition()
	local itemY = itemEntry:getYPosition()
	local offset = itemEntry:getPixelSize().height/2 - 
		self.arrow:getPixelSize().height/2
	
	self.arrow:setYPosition(baseY + itemY + Utils.newUDim(0, offset)) 
	Utils.fireEvent("ArrowAnimStart", self.arrow)
end

function ParcelView:_stopArrowAnim()
	Utils.fireEvent("ArrowAnimStop", self.arrow)
	self.arrow:setProperty("Image", "")
end

function ParcelView:_changeArrowAnim(itemEntry)
	self:_stopArrowAnim()
	self:_startArrowAnim(itemEntry)
end

function ParcelView:_isItemEntrySelected()
	for i=1,self.listbox:getItemCount() do
		local item = self.listbox:getItemFromIndex(i - 1)
		if item:isSelected() then 
			return i - 1 
		end
	end
end

---------------------------------------------------------------
	-- public method
---------------------------------------------------------------
function ParcelView:init(color, offset, clickHandler)
	local arrowSz = self.arrow:getPixelSize()
	self.arrow:setXPosition(Utils.newUDim(0.5, offset*arrowSz.width))
	
	local frameComponent = { "LT", "RT", "LB", "RB", "L", "R", "T", "B", "M", }
	for i=1, self.listbox:getItemCount() do
		local item = self.listbox:getItemFromIndex(i-1)
		item:setSelectable(true)
		for _,fc in ipairs(frameComponent) do
			item:setProperty("HoverFrame"..fc, 
				"XihadUI/Menu/"..color.."_hover_item"..fc)
		end
		
		local functional = require 'std.functional'
		subscribeEvent(item, "Clicked", clickHandler)
		subscribeEvent(item, "MouseEntersArea", 
			functional.bindself(self, "_onItemHover"))
		
		subscribeEvent(item, "MouseLeavesArea", function (e)
			if not CEGUI then return end
			self:_onItemHoverNil(e)
		end)
	end
end

function ParcelView:tidyParcel()
	self.listbox:clearAllSelections()
	self:_stopArrowAnim()
	self.parcel:tidy()
	self:_notifyDataChange()
end

function ParcelView:setModel(warriorName, parcel)
	self.label:setText(string.format("<%s>", warriorName))
	self.parcel = parcel
	self:_notifyDataChange()
	self.listbox:sizeToContent()
end

function ParcelView:reset()
	self.listbox:clearAllSelections()
	self:_stopArrowAnim()
	self.parcel = nil
	self:_notifyDataChange()
end

function ParcelView:relayout(adjustedWidth)
	local size = self.listbox:getPixelSize()
	local wndSz = self.listbox:getParent():getParentPixelSize()
	
	self.listbox:setWidth(Utils.newUDim(0, adjustedWidth))
	self.listbox:setXPosition(Utils.newUDim(self.scaleX, -0.5*adjustedWidth))
	self.listbox:setYPosition(Utils.newUDim(0, 0.52*0.9*wndSz.height-0.5*size.height))
end

function ParcelView:_onItemHover(e)
	local itemEntry = CEGUI.toMenuItem(CEGUI.toWindowEventArgs(e).window)
	if itemEntry:getOwnerList() == self.listbox then
		if itemEntry:isDisabled() or self:_isItemEntrySelected() then 
			return
		end
		self:_startArrowAnim(itemEntry)
	end
end

function ParcelView:_onItemHoverNil(e)
	local itemEntry = CEGUI.toMenuItem(CEGUI.toWindowEventArgs(e).window)
	if itemEntry:getOwnerList() == self.listbox then
		if itemEntry:isDisabled() or self:_isItemEntrySelected() then 
			return
		end
	
		self:_stopArrowAnim()
	end
end

local function tryExchangeDirectly(current, another, clickedItem)
	local index = clickedItem:getOwnerList():getItemIndex(clickedItem) + 1
	
	local item = current.parcel:getItemAt(index)
	if not item then 
		clickedItem:setSelectable(false)
		return false
	end
	
	if pcall(another.parcel.gainItem, another.parcel, item, 1) then 
		current.parcel:dropItemAt(index, 1)
		clickedItem:setSelectable(false)
		-- print("exchange directly")
		return true
	else
		-- print("can't exchange directly, must select a slot to exchange!")
		clickedItem:setSelectable(true)
		return false
	end
end

function ParcelView:onItemClick(clickedEntry, other)
	if clickedEntry:getOwnerList() ~= self.listbox then 
		return 
	end
	
	local index = clickedEntry:getOwnerList():getItemIndex(clickedEntry)
	
	local selected = self:_isItemEntrySelected()
	local otherSelected = other:_isItemEntrySelected()
	
	assert(not selected or not otherSelected)
	if selected and selected == index then 
		-- print("toggle selected item, return")
		return 
	end
	
	if not otherSelected then
		if selected then 
			self.listbox:clearAllSelections()
			self:_changeArrowAnim(clickedEntry)
		end
		if not tryExchangeDirectly(self, other, clickedEntry) then
			return
		end
	else	-- otherSelected and not selected then swap slots
		self.parcel:swapWith(other.parcel, index + 1, otherSelected + 1)
		clickedEntry:setSelectable(false)
		self.listbox:clearAllSelections()
		other.listbox:clearAllSelections()
		other:_stopArrowAnim()
	end
	
	self:_notifyDataChange()
	other:_notifyDataChange()
end

return ParcelView
