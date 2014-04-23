local base = require 'Controller.PlayerControlState'

local ChooseTargetState = {}
ChooseTargetState.__index = ChooseTargetState
setmetatable(ChooseTargetState, base)

function ChooseTargetState.new()
	local obj = setmetatable(base.new(), ChooseTargetState)
	return obj
end

-- TODO
function ChooseTargetState:onVacancySelected(tileObject) 
end

function ChooseTargetState:onHeroSelected(heroObject) 
end

function ChooseTargetState:onEnemySelected(enemyObject) 
end

return ChooseTargetState