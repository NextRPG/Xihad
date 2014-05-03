local Trigonometry = require 'math.Trigonometry'
local SpanVariable = require 'Action.SpanVariable'
local ObjectAction = require 'ObjectAction'
local Location = require 'route.Location'

local MovementBuilder = {
	currentLocation = nil,
	currentAngle = nil,
	object = nil,
}
MovementBuilder.__index = MovementBuilder

function MovementBuilder.new(object)
	local warrior = object:findComponent(c'Warrior')
	local builder = setmetatable({
			object = object,
			currentLocation = warrior:getLocation(),
		}, MovementBuilder)
	
	local _, y, _ = object:getRotation():xyz()
	builder.currentAngle = y
	
	return builder
end

function MovementBuilder:rotate(targetAngle)
	if not Trigonometry.equal(self.currentAngle, targetAngle) then
		local spanY = SpanVariable.new(self.currentAngle, targetAngle)
		self.currentAngle = targetAngle
		return ObjectAction.rotateY(self.object, spanY, 90/0.1)
	end
end

function MovementBuilder:_rotate(targetAngle, actionArray)
	local action = self:rotate(targetAngle)
	
	if action then
		table.insert(actionArray, action)
	end
end

function MovementBuilder:toVector(location)
	local tile = g_chessboard:getTile(location)
	
	if not tile then
		error('no such location', 1)
	else
		return tile:getCenterVector()
	end
end

function MovementBuilder:moveDirectly(location)
	local currentPos = self:toVector(self.currentLocation)
	local nextPos = self:toVector(location)
	
	local spanTranslate = SpanVariable.new(currentPos, nextPos)
	self.currentLocation:set(location.x, location.y)
	return ObjectAction.move(self.object, spanTranslate, 35)
end

function MovementBuilder:move(nextLocation, actionArray)
	local currentLocation = self.currentLocation
	local currentAngle = self.currentAngle
	
	local deltaAngle = 0
	if nextLocation.x > currentLocation.x then
		self:_rotate(90, actionArray)
	elseif nextLocation.x < currentLocation.x then
		self:_rotate(-90, actionArray)
	end
	
	if nextLocation.x ~= currentLocation.x then
		local target = Location.new(nextLocation.x, currentLocation.y)
		local moveX = self:moveDirectly(target)
		table.insert(actionArray, moveX)
	end
	
	if nextLocation.y > currentLocation.y then
		self:_rotate(0, actionArray)
	elseif nextLocation.y < currentLocation.y then
		self:_rotate(-180, actionArray)
	end
	
	if nextLocation.y ~= currentLocation.y then
		local target = Location.new(currentLocation.x, nextLocation.y)
		local moveY = self:moveDirectly(target)
		table.insert(actionArray, moveY)
	end
end

return MovementBuilder