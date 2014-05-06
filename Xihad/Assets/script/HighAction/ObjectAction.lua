local SpanVariable = require 'SpanVariable'
local SpanAction   = require 'SpanVariableAction'
local functional   = require 'std.functional'

local actions = {}

function actions.move(object, spanVector, duration, lerp)
	spanVector.origin = spanVector.origin or object:getTranslate()
	local sync= functional.bindself(object, 'resetTranslate')
	return SpanAction.new(spanVector, sync, duration, lerp)
end

function actions.rotateY(object, spanY, duration, lerp)
	if not spanY.origin then
		local _, currentY, _ = object:getRotation():xyz()
		spanY.origin = currentY
	end
	
	local sync= function (currentY)
		local curr = object:getRotation()
		curr:set(nil, currentY)
		object:resetRotation(curr)
	end
	
	return SpanAction.new(spanY, sync, duration, lerp)
end

return actions
