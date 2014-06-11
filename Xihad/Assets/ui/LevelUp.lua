local Utils = require "ui.StaticUtils"

local LevelUp = {}
LevelUp.window = Utils.findWindow("LevelUp")
LevelUp.lastLevel = LevelUp.window:getChild("LArea/LastLevel")
LevelUp.arrow = LevelUp.window:getChild("MArea/Arrow")
LevelUp.newLevel = LevelUp.window:getChild("RArea/NewLevel")

function LevelUp:show(currrentLevel, nextLevel, powerUpList)
	Utils.fireEvent("_Start", self.lastLevel)
	Utils.fireEvent("_Start", self.arrow)
	Utils.fireEvent("_Start", self.newLevel)
	return self.window:getParent()
end

function LevelUp:close()
end

function LevelUp:init()
end

return LevelUp