local base = require "Action.TimedAction"

local VarAction = {
	spanVariable = nil,
	assignmenter = nil,
}
VarAction.__index = VarAction
setmetatable(VarAction, base)

function VarAction.new(spanVariable, assignmenter, duration, lerpfunc)
	local o = base.new(duration, easefunc)
	o.spanVariable = spanVariable
	o.assignmenter = assignmenter
	assert(o.spanVariable)
	
	setmetatable(o, VarAction)
	return o
end

function VarAction:onProgress(progress)
	local current = self.spanVariable:interpolate(progress)
	assert(current, "interpolate function returned nil value")
	self.assignmenter(current)
end

return VarAction
