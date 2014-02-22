Sequence = {
	
}

package.path = package.path .. ";../?.lua"
require "LuaUtils"

function Sequence.new( o )
	o = o or {}
	setmetatable(o, {__index = Sequence})
	return o
end

function Sequence:runActions( actions, callback )
	-- print(#actions)
	for i,action in ipairs(actions) do
		if actions[i + 1] ~= nil then
			action.callback = function () 
				self.object:findComponent(c(actions[i + 1].actionType)):runAction(actions[i + 1]) 
			end
		else 
			if callback ~= nil then
				action.callback = callback
			end
		end
	end
	self.object:findComponent(c(actions[1].actionType)):runAction(actions[1])

	-- print( " 4 " , coroutine.running())
end

function Sequence:runMoveActions( actions, callback )
	-- local move = self.object:findComponent(c"MoveBy")
	-- local rotate = self.obejct:appendComponent(c"Action.RotateBy")
	local newActions = {}

	local rx, ry, rz = self.object:getRotation():xyz()
	function rotateBeforeMove( index ) 
		local ty = getLogicAngle(actions[index].destination)
		local rotate = {destination = {y = calRotation( ry, ty )},
		 interval = 0.25, actionType = "RotateBy"}
		 ry = ty
		newActions[#newActions + 1] = rotate 
		-- start.callback = function ( ) move:runAction(actions[index]) end
	end

	for i,action in ipairs(actions) do
		rotateBeforeMove(i)
		action.actionType = "MoveBy"
		newActions[#newActions + 1] = action
	end

	-- print(newActions[1].actionType)

	self:runActions(newActions, callback)

end

return Sequence