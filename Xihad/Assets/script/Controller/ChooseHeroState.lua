local base = require 'Controller.PlayerState'
local ChooseHeroState = setmetatable({}, base)
ChooseHeroState.__index = ChooseHeroState

function ChooseHeroState.new(...)
	return setmetatable(base.new(...), ChooseHeroState)
end

function ChooseHeroState:onStateEnter(state, prev)
	self:_focusObject(nil)
end

function ChooseHeroState:onVacancySelected(tile)
	-- show tile info
	self:_showTileInfo(tile)
	
	self:_fastenCursorWhen(function ()
		self:_focusTile(tile)
	end)
end

function ChooseHeroState:onHeroSelected(heroObject)
	local warrior = heroObject:findComponent(c'Warrior')
	self:_showWarriorInfo(warrior)
	
	if not warrior:isActive() then 
		print('Warrior is not active')
	else
		self.commandList:setSource(warrior)
		return 'next'
	end
end

function ChooseHeroState:onEnemySelected(enemyObject)
	-- mark range
	self:_showWarriorInfo(enemyObject:findComponent(c'Warrior'))
	
	self:_fastenCursorWhen(function ()
		self:_focusObject(enemyObject)
	end)
	
	-- TODO
	-- self.painter:showRange(enemyObject)
end

return ChooseHeroState
