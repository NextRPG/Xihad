local ObjectAction = require 'ObjectAction'
local ActionFactory= require 'ActionFactory'
local ActionAdapter= require 'ActionAdapter'
local SpanVariable = require 'SpanVariable'
local movement = {}

function movement.moveToTarget(object, moveDirections)
	local newActions = {}
	
	local _, currentY, _ = object:getRotation():xyz()
	local originPos= object:getTranslate() 
	for i,action in ipairs(moveDirections) do
		local finishY = getLogicAngle(action.destination)
		local rotation = calRotation(currentY, finishY)
		if rotation ~= 0 then
			local spanY = SpanVariable.newWithDelta(currentY, rotation)
			local rotAction = ObjectAction.rotateY(object, spanY, 0.2)
			table.insert(newActions, rotAction)
			currentY = finishY
		end
		
		local delta = math3d.vector(action.destination.x * Consts.TILE_WIDTH, 0, action.destination.y * Consts.TILE_HEIGHT)
		local spanTranslate = SpanVariable.newWithDelta(originPos, delta)
		local move = ObjectAction.move(object, spanTranslate, action.interval)
		table.insert(newActions, move)
		originPos = spanTranslate.finish
	end
	
	local sequence = ActionFactory.sequence(newActions)
	ActionAdapter.fit(object, sequence)
	
	return sequence
end

return movement
