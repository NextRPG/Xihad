local PlayerState = {}

function PlayerState:onBack()
end

function PlayerState:onTouch(x, y)
	local obj = g_collision:detect(g_collision:getRayFromScreenCoord(x, y))
	if obj then
		return self:onObjectSelected(obj)
	end
end

function PlayerState:onHover(x, y)
	local obj = g_collision:detect(g_collision:getRayFromScreenCoord(x, y))
	if obj then
		return self:onObjectHovered(obj)
	end
end

function PlayerState:onObjectSelected(object)
	-- test if it is a tile or person
	if object:hasTag(c'Tile') then
		return self:onTileSelected(object)
	elseif object:hasTag(c'Character')
		return self:onPersonSelected(object)
	else
		print('Non-Tile and Non-Character object found')
	end
end

function PlayerState:onObjectHovered(object)
	
end

function PlayerState:onTileSelected(tileObject)
	-- if there is a person standing on the tile, then invoke onPersonSelected()
	-- else invoke onVacancySelected()
end

function PlayerState:onVacancySelected(tileObject)
	
end

function PlayerState:onPersonSelected(personObject)
	if personObject:hasTag(c'Hero') then
		return self:onHeroSelected(personObject)
	elseif personObject:hasTag(c'AI') then
		return self:onEnemySelected(personObject)
	else
		print('Non-hero and Non-AI person found')
	end
end

function PlayerState:onHeroSelected(heroObject)
	-- Override only
end

function PlayerState:onEnemySelected(enemyObject)
	-- Override only
end

function PlayerState:onUICommand(command)
	-- Override only
end
