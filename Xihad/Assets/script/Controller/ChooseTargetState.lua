local base = require 'Controller.PlayerState'

local ChooseTargetState = {}
ChooseTargetState.__index = ChooseTargetState
setmetatable(ChooseTargetState, base)

function ChooseTargetState.new(...)
	return setmetatable(base.new(...), ChooseTargetState)
end


function ChooseTargetState:needCDWhenTouch()
	return false
end

function ChooseTargetState:needCDWhenHover()
	return false
end

function ChooseTargetState:onTileSelected(tile)
	-- self.executor:cast()
	-- attack
	self.camera:focus(nil)
end

function ChooseTargetState:onTileHovered(tile)
	-- show all attackable enemies' status
end

return ChooseTargetState
