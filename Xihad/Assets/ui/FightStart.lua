local Utils = require "ui.StaticUtils"
local ParcelView = require "ui.ParcelView"

local FightStart = {}

function FightStart:_relayout(text, Shadow)
	local sz = Utils.sizeToWrapText(Shadow)
	Shadow:setSize(sz)
	text:setSize(sz)
	return sz
end

function FightStart:show(victoryCondition)
	assert(type(victoryCondition) == "string")
	
	self.conText:setText(victoryCondition)
	self.conShadow:setText(victoryCondition)
	self:_relayout(self.conText, self.conShadow)
	
	Utils.fireEvent("_Start", self.window)
	Utils.fireEvent("_Start", self.vicShadow)
	Utils.fireEvent("_Start", self.conShadow)
	return self.window:getParent()
end

function FightStart:close()
end

function FightStart:init()
	local padding = 5
	local panel = Utils.findWindow("FightStartPanel")
	self.window = panel:getChild("FightStart")
	self.vicShadow = self.window:getChildRecursive("VictoryShadow")
	self.vicText = self.vicShadow:getChild("Victory")
	self.conShadow = self.window:getChildRecursive("ConditionShadow")
	self.conText = self.conShadow:getChild("Condition")
	local sz = panel:getPixelSize()

	local textSz = self:_relayout(self.vicText, self.vicShadow)
	local panelT = self.vicShadow:getParent()
	local panelB = self.conShadow:getParent()
	
	panelT:setSize(CEGUI.USize:new(Utils.newUDim(1, 0), textSz.height))
	panelB:setSize(CEGUI.USize:new(Utils.newUDim(1, 0), textSz.height))
	panelT:setYPosition(
		Utils.newUDim(0, sz.height/2-padding-textSz.height.offset))	
	panelB:setYPosition(Utils.newUDim(0, sz.height/2+padding))
end

return FightStart