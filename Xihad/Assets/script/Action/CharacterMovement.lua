local ObjectAction = require 'ObjectAction'
local ActionFactory= require 'ActionFactory'
local ActionAdapter= require 'ActionAdapter'
local movement = {}

function movement.moveToTarget(object, moveDirections)
	local newActions = {}
	
	local _, currentY, _ = object:getRotation():xyz()
	local originPos= object:getTranslate() 
	for i,action in ipairs(moveDirections) do
		local finishY = getLogicAngle(action.destination)
		local rotation = calRotation(currentY, finishY)
		if rotation ~= 0 then
			local finishY = currentY + rotation
			local rotAction = ObjectAction.rotateY(object, currentY, finishY, 0.2)
			table.insert(newActions, rotAction)
			currentY = finishY
		end
		
		local finishPos = originPos + 
			math3d.vector(action.destination.x * Consts.TILE_WIDTH, 0, action.destination.y * Consts.TILE_HEIGHT)
		local move = ObjectAction.move(object, originPos, finishPos, action.interval)
		table.insert(newActions, move)
		originPos = finishPos
	end
	
	local sequence = ActionFactory.sequence(newActions)
	object:appendUpdateHandler(ActionAdapter.new(sequence))	-- update it
	
	return sequence
end

return movement
