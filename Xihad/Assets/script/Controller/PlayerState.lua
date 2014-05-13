local Class = require 'std.Class'

local PlayerState = {
	ui = nil,
	camera = nil,
	painter = nil,
	executor = nil,
	commandList = nil,
	touchDispatcher = nil,
	hoverDispatcher = nil,
}
PlayerState.__index = PlayerState

function PlayerState.new(sharedCommandList, newDispatcher, ui, camera, painter, executor)
	local o = setmetatable({
			ui = ui,
			camera = camera,
			painter = painter, 
			executor = executor,
			commandList = sharedCommandList,
		}, PlayerState)
	assert(sharedCommandList ~= nil)
	
	local touchListener = {}
	Class.delegateClosure(touchListener, 'needCollisionDetection', o, 'needCDWhenTouch')
	Class.delegateClosure(touchListener, 'onWarrior', o, 'onWarriorSelected')
	Class.delegateClosure(touchListener, 'onVacancy', o, 'onVacancySelected')
	Class.delegateClosure(touchListener, 'onTile' ,   o, 'onTileSelected')
	Class.delegateClosure(touchListener, 'onHero' ,   o, 'onHeroSelected')
	Class.delegateClosure(touchListener, 'onEnemy',   o, 'onEnemySelected')
	o.touchDispatcher = newDispatcher(touchListener)
	
	local hoverListener = {}
	Class.delegateClosure(hoverListener, 'needCollisionDetection', o, 'needCDWhenHover')
	Class.delegateClosure(hoverListener, 'onWarrior', o, 'onWarriorHovered')
	Class.delegateClosure(hoverListener, 'onVacancy', o, 'onVacancyHovered')
	Class.delegateClosure(hoverListener, 'onTile' ,   o, 'onTileHovered')
	Class.delegateClosure(hoverListener, 'onHero' ,   o, 'onHeroHovered')
	Class.delegateClosure(hoverListener, 'onEnemy',   o, 'onEnemyHovered')
	o.hoverDispatcher = newDispatcher(hoverListener)
	
	return o
end

function PlayerState:_safeClear(handleField)
	self.painter:clear(self[handleField])
	self[handleField] = nil
end

function PlayerState:onStateEnter(state, prev)
end

function PlayerState:onStateExit(state, next)
end

function PlayerState:needCDWhenTouch()
	return true
end

function PlayerState:needCDWhenHover()
	return true
end

function PlayerState:onBack()
	return 'back'
end

function PlayerState:onTouch(x, y, times)
	return self.touchDispatcher:dispatch(x, y, times)
end

function PlayerState:_onTile(tile, times, warriorFunc, vacancyFunc)
	local warrior = tile:getWarrior()
	if warrior then
		return self[warriorFunc](self, warrior:getHostObject(), times)
	else
		return self[vacancyFunc](self, tile, times)
	end
end

function PlayerState:_onWarrior(warriorObj, heroFunc, enemyFunc)
	if warriorObj:hasTag(c'Hero') then
		return self[heroFunc](self, warriorObj)
	elseif warriorObj:hasTag(c'Enemy') then
		return self[enemyFunc](self, warriorObj)
	else
		print('Non-hero and Non-AI person found')
	end
end

function PlayerState:onWarriorSelected(warriorObj)
	return self:_onWarrior(warriorObj, 'onHeroSelected', 'onEnemySelected')
end

function PlayerState:onTileSelected(tile, times)
	return self:_onTile(tile, 'onWarriorSelected', 'onVacancySelected')
end

function PlayerState:onVacancySelected(tileObject) 
end

function PlayerState:onHeroSelected(heroObject) 
end

function PlayerState:onEnemySelected(enemyObject) 
end

function PlayerState:onHover(x, y)
	return self.hoverDispatcher:dispatch(x, y)
end

function PlayerState:onWarriorHovered(warriorObj)
	return self:_onWarrior(warriorObj, 'onHeroHovered', 'onEnemyHovered')
end

function PlayerState:onTileHovered(tile)
	return self:_onTile(tile, nil, 'onWarriorHovered', 'onVacancyHovered')
end

function PlayerState:onVacancyHovered(tileObject) 
end

function PlayerState:onHeroHovered(heroObject) 
end

function PlayerState:onEnemyHovered(enemyObject) 
end

function PlayerState:onUICommand(command)
end

return PlayerState
