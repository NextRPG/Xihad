local Trigonometry = require 'math.Trigonometry'
local SpanAction   = require 'Action.SpanVariableAction'
local functional   = require 'std.functional'

local ObjectAction = {}

function ObjectAction.move(object, spanVector, speed, lerp)
	spanVector.origin = spanVector.origin or object:getTranslate()
	local duration = spanVector:delta():length() / speed
	local sync= functional.bindself(object, 'resetTranslate')
	return SpanAction.new(spanVector, sync, duration, lerp)
end

function ObjectAction.rotateY(object, spanY, rotSpeed, lerp)
	if not spanY.origin then
		local _, currentY, _ = object:getRotation():xyz()
		spanY.origin = currentY
	end
	
	local delta = Trigonometry.normalize(spanY:delta())
	spanY.finish= spanY.origin + delta
	
	local sync= function (currentY)
		local curr = object:getRotation()
		curr:set(nil, currentY)
		object:resetRotation(curr)
	end
	
	return SpanAction.new(spanY, sync, math.abs(delta) / rotSpeed, lerp)
end

return ObjectAction
