local MapElement = require 'route.MapElement'

local Character = {
	team = nil,
	career = nil,
	currentExp = 0,
	properties = nil,
	equipments = nil,
	attachedStates = nil,
}
Character.__index = Character
setmetatable(Character, MapElement)

function Character.new( data, object )
	object:addTag(c(data.team))
	object:addTag(c'Character')
end

function Character:isLeagueWith(other)
	return self.team == other.team
end

function Character:canPass( character )
	return self:isLeagueWith(character)
end

function Character:canStay( character )
	return character == self
end

function Character:permitCasting( character, skill )
	if self == character then
		return skill:canCastToSelf()
	elseif self:isLeagueWith(character) then
		return skill:canCastToLeague()
	else
		return skill:canCastToEnemty()
	end
end

function Character:setTile( tile ) 
	-- ;;;;;
	MapElement.setTile(self, tile)
end

