local base = require 'Controller.PlayerControlState'

local ChooseHeroState = setmetatable({}, base)
ChooseHeroState.__index = ChooseHeroState

function ChooseHeroState.new()
	return setmetatable(base.new(), ChooseHeroState)
end

function ChooseHeroState:onVacancySelected(tileObject)
	-- show tile info
	self.ui:showInfo(tileObject)
end

function ChooseHeroState:onHeroSelected(heroObject)
	self.ui:showInfo(heroObject)
	self.chessboard:showRange(heroObject)
	self.camera:focus(heroObject)
	
	if heroObject:isActive() then
		self.commandList:setSource(heroObject)
		return 'next'
	end
end

function ChooseHeroState:onEnemySelected(enemyObject)
	-- mark range
	self.ui:showInfo(heroObject)
	self.chessboard:showRange(enemyObject)
end

return ChooseHeroState
