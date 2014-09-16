local Utils = require "ui.StaticUtils"
local ParcelView = require "ui.ParcelView"

local Progress = {
	callback = nil
}

local function checkRange(progress)
	assert(progress <= 1.0 and progress >= 0)
end

function Progress:show(currentProgress, targetProgress, callback, isOverStep)
	checkRange(currentProgress)
	checkRange(targetProgress)
	
	self.window:setProgress(currentProgress)
	self.window:setProperty("TargetProgress", targetProgress)
	self.window:setProperty("CurrentProgressValue", math.floor(currentProgress*100))
	self.window:setProperty("TargetProgressValue", math.floor(targetProgress*100))
	
	self.callback = callback
	Utils.fireEvent("_Open", self.window)
	Utils.fireEvent(isOverStep and "_GrowOver" or "_Grow", self.window)
	return self.window:getParent()
end

function Progress:close()
	Utils.fireEvent("_Close", self.window)
end

function Progress:init()
	local parent = Utils.findWindow("GainExpPanel")
	self.window = CEGUI.toProgressBar(parent:getChild("GainExp"))
end

function Progress:onAnimationEnded(e)
	print('Progress::onAnimationEnded')
	if self.callback then
		self.callback()
	end
end

return Progress