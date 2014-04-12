local ChooseTileState = {}

function ChooseTileState:onBack()
	-- back to ChooseHeroState
	Chessboard:clearAll()
	to(ChooseHeroState)
end

function ChooseTileState:onVacancySelected(tileObject)
	-- show tile info
	
	if PathFinder:hasTile(object:findComponent(c"Tile")) then
		self.manager:onSelectTile(object:findComponent(c"Tile"))
		SkillManager:onShowSkills(self.manager.currentCharacter)
	else
		
	end
end

function ChooseTileState:onHeroSelected(heroObject)
	-- mark range 
	-- change state
	self.manager:onSelectCharacter(object)
end

function ChooseTileState:onEnemySelected(enemyObject)
	-- mark range
	self.manager:onSelectCharacter(object)
end

function ChooseTileState:onUICommand(command)
	-- ignore
end
