local base = require 'Controller.PlayerState'
local ChooseHeroState = setmetatable({}, base)
ChooseHeroState.__index = ChooseHeroState

function ChooseHeroState.new(...)
	return setmetatable(base.new(...), ChooseHeroState)
end

function ChooseHeroState:onVacancySelected(tile)
	-- show tile info
	self.ui:showTileInfo(tile)
end

function ChooseHeroState:onHeroSelected(heroObject)
	self.ui:showWarriorInfo(heroObject)
	
	local warrior = heroObject:findComponent(c'Warrior')
	if not warrior:isActive() then 
		print('Warrior is not active')
		return
	end
	
	local reachable = g_chessboard:getReachableTiles(warrior)
	self.commandList.reachableHandle = self.painter:mark(reachable, 'Reachable')
	self.camera:focus(heroObject)
	self.commandList:setSource(warrior)
	return 'next'
end

function ChooseHeroState:onEnemySelected(enemyObject)
	-- mark range
	self.ui:showWarriorInfo(enemyObject)
	
	-- TODO
	self.painter:showRange(enemyObject)
end

return ChooseHeroState
