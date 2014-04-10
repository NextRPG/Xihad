local ChooseCommandState = {}

function ChooseCommandState:onBack()
	self.manager:back2ShowCharacter()
	return 'back'
end

function ChooseCommandState:onVacancySelected(tileObject)
	-- ignore
end

function ChooseCommandState:onHeroSelected(heroObject)
	-- ignore
end

function ChooseCommandState:onEnemySelected(enemyObject)
	-- ignore
end

function ChooseCommandState:onUICommand(command)
	if command == 'Standby' then
		return 'fail'
	else
		SkillManager:onSelectSkill(key)
	end
end
