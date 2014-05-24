local ObjectAction = require 'HighAction.ObjectAction'
local ActionFactory= require 'Action.ActionFactory'
local SpanVariable = require 'Action.SpanVariable'
local MovementBuilder = require 'HighAction.MovementBuilder'
local WarriorMovement = {}

function WarriorMovement.moveOnPath(object, path)
	local newActions = {}
	
	local actionBuilder = MovementBuilder.new(object)
	for _, location in ipairs(path) do
		actionBuilder:move(location, newActions)
	end
	
	return ActionFactory.sequence(newActions)
end

function WarriorMovement.moveToLocation(object, location, checkCost)
	local warrior = object:findComponent(c'Warrior')
	
	if warrior:getLocation() == location then
		return nil
	end
	
	local path, cost = g_chessboard:route(warrior, location)
	
	if not path then
		return nil
	end
	
	if checkCost and cost > warrior:getActionPoint() then
		return nil
	end
	
	local action = WarriorMovement.moveOnPath(object, path)
	action:addFinishListener(function ()
		local barrier = object:findComponent(c'Barrier')
		barrier:setTile(g_chessboard:getTile(location))
	end)
	
	return action
end

return WarriorMovement
