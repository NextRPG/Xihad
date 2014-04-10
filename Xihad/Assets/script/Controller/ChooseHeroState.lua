local ChooseHeroState = {}

function ChooseHeroState:onBack()
	-- ignore
end

function ChooseHeroState:onVacancySelected(tileObject)
	-- show tile info
end

function ChooseHeroState:onHeroSelected(heroObject)
	self.manager:onSelectCharacter(object)
	return 'next'
end

function ChooseHeroState:onEnemySelected(enemyObject)
	-- mark range
	self.manager:onSelectCharacter(object)
end

function ChooseHeroState:onUICommand(command)
	-- ignore
end
