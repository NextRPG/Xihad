Sequence = {
	
}

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
				self.object:findComponent(c(actions[i + 1].actionType)):runAction(actions[i + 1], actions[i + 1].callback) 
			end
		elseif callback ~= nil then
			action.callback = callback
		end
	end

	self.object:findComponent(c(actions[1].actionType)):runAction(actions[1], actions[1].callback)
end

function Sequence:runMoveActions( actions, callback )

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

	self:runActions(newActions, callback)

end

return Sequence