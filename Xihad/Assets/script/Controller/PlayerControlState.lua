local PlayerState = {
	touchEventDispatcher = nil,
	hoverEventDispatcher = nil,
}
PlayerState.__index = PlayerState

function PlayerState.new()
	local o = setmetatable({}, PlayerState)
	
	local touchListener = {}
	Class.delegateClosure(touchListener, 'onWarrior', o, 'onWarriorSelected')
	Class.delegateClosure(touchListener, 'onVacancy', o, 'onVacancySelected')
	Class.delegateClosure(touchListener, 'onTile' ,   o, 'onTileSelected')
	Class.delegateClosure(touchListener, 'onHero' ,   o, 'onHeroSelected')
	Class.delegateClosure(touchListener, 'onEnemy',   o, 'onEnemySelected')
	
	local hoverListener = {}
	Class.delegateClosure(hoverListener, 'onWarrior', o, 'onWarriorHovered')
	Class.delegateClosure(hoverListener, 'onVacancy', o, 'onVacancyHovered')
	Class.delegateClosure(hoverListener, 'onTile' ,   o, 'onTileHovered')
	Class.delegateClosure(hoverListener, 'onHero' ,   o, 'onHeroHovered')
	Class.delegateClosure(hoverListener, 'onEnemy',   o, 'onEnemyHovered')
	
	o.touchEventDispatcher = MouseEventDispatcher.new(touchListener),
	o.hoverEventDispatcher = MouseEventDispatcher.new(hoverListener),
	return o
end

function PlayerState:onBack()
end

function PlayerState:onTouch(x, y)
	return self.touchEventDispatcher:dispatch(x, y)
end

Class.delegate(PlayerState, 'onWarriorSelected', 'touchEventDispatcher', 'onWarriorDefault')

Class.delegate(PlayerState, 'onTileSelected', 'touchEventDispatcher', 'onTileDefault')

function PlayerState:onVacancySelected(tileObject) 
end

function PlayerState:onHeroSelected(heroObject) 
end

function PlayerState:onEnemySelected(enemyObject) 
end

function PlayerState:onHover(x, y)
	return self.hoverEventDispatcher:dispatch(x, y)
end

Class.delegate(PlayerState, 'onWarriorHovered', 'hoverEventDispatcher', 'onWarriorDefault')

Class.delegate(PlayerState, 'onTileHovered', 'hoverEventDispatcher', 'onTileDefault')

function PlayerState:onVacancyHovered(tileObject) 
end

function PlayerState:onHeroHovered(heroObject) 
end

function PlayerState:onEnemyHovered(enemyObject) 
end


function PlayerState:onUICommand(command)
end

return PlayerState
