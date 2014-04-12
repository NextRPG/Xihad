local SpanVariable = require 'SpanVariable'
local SpanAction   = require 'SpanVariableAction'
local functional   = require 'std.functional'

local actions = {}

function actions.move(object, origin, finish, duration, lerp)
	origin = origin or object:getTranslate()
	local var = SpanVariable.new(origin, finish)
	local sync= functional.bindself(object, 'resetTranslate')
	return SpanAction.new(var, sync, duration, lerp)
end

function actions.rotateY(object, originY, finishY, duration, lerp)
	if not originY then
		local _, currentY, _ = object:getRotation():xyz()
		originY = currentY
	end
	
	local var = SpanVariable.new(originY, finishY)
	local sync= function (currentY)
		local curr = object:getRotation()
		curr:set(nil, currentY)
		object:resetRotation(curr)
	end
	
	return SpanAction.new(var, sync, duration, lerp)
end

return actions
