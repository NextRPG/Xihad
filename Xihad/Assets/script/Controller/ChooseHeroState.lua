local base = require 'Controller.PlayerState'
local Table= require 'std.Table'
local ChooseHeroState = setmetatable({
		visited = nil,
		enemyHandle = nil,
	}, base)
ChooseHeroState.__index = ChooseHeroState

function ChooseHeroState.new(...)
	local o = setmetatable(base.new(...), ChooseHeroState)
	o.visited = {}
	return o
end

function ChooseHeroState:_shouldFocus(warrior)
	return not self.visited[warrior] and warrior:isActive()
end

function ChooseHeroState:_findNextFocus()
	for object in g_scene:objectsWithTag('Hero') do
		local warrior = object:findComponent(c'Warrior')
		assert(warrior)
		
		if self:_shouldFocus(warrior) then 
			self.visited[warrior] = true
			return object
		end
	end
end

function ChooseHeroState:_clearVisited()
	Table.clear(self.visited)
end

function ChooseHeroState:traverseFocus()
	local nextFocus = self:_findNextFocus()
	
	if not nextFocus then
		self:_clearVisited()
		nextFocus = self:_findNextFocus()
	end
	
	assert(nextFocus)
	self:_focusObject(nextFocus)
end

function ChooseHeroState:onBack()
	self:_showTileInfo(nil)
	self:traverseFocus()
end

function ChooseHeroState:onStateEnter(state, prev)
	self:_focusObject(nil)
	
	if prev == 'Finish' then
		self:_clearVisited()
	end
end

function ChooseHeroState:onVacancySelected(tile)
	-- show tile info
	self:_showTileInfo(tile)
	
	self:_fastenCursorWhen(function ()
		self:_focusTile(tile)
	end)
end

function ChooseHeroState:_showEnemyAttackRange(warrior)
	local skillCaster = warrior:findPeer(c'SkillCaster')
	local set = {}
	for _, tile in ipairs(g_chessboard:getReachableTiles(warrior)) do
		skillCaster:getCastableTiles(tile:getLocation(), set)
	end
	
	self:_markRange(Table.extractKeys(set), 'Attack', 'enemyHandle')
end

function ChooseHeroState:_promoteWarrior(warrior)
	self:_safeClear('enemyHandle')
	
	self:_showTileInfo(warrior:findPeer(c'Barrier'):getTile())
	self:_showWarriorInfo(warrior)
end

function ChooseHeroState:_promoteEnemy(warrior)
	self:_promoteWarrior(warrior)
	self:_showEnemyAttackRange(warrior)
end

function ChooseHeroState:onHeroSelected(heroObject)
	local warrior = heroObject:findComponent(c'Warrior')
	self:_promoteWarrior(warrior)
	
	if not warrior:isActive() then 
		print('Warrior is not active')
	else
		self.commandList:setSource(warrior)
		return 'next'
	end
end

function ChooseHeroState:onEnemySelected(enemyObject)
	-- mark range
	self:_safeClear('enemyHandle')
	self:_promoteEnemy(enemyObject:findComponent(c'Warrior'))
	
	self:_fastenCursorWhen(function ()
		self:_focusObject(enemyObject)
	end)
end

return ChooseHeroState
