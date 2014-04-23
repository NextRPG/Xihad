local base = require 'Barrier'

local WarriorBarrier = {
	team = nil,
}
WarriorBarrier.__index = WarriorBarrier
WarriorBarrier.__base = 'Barrier'
setmetatable(WarriorBarrier, base)

function WarriorBarrier.new(data, object)
	local o = {}
	o.team = data.team
	setmetatable(o, WarriorBarrier)
	return o
end

function WarriorBarrier:isLeagueWith(other)
	return self.team == other.team
end

function WarriorBarrier:canPass( character )
	return self:isLeagueWith(character)
end

function WarriorBarrier:canStay( character )
	return character == self
end

function WarriorBarrier:permitCasting( character, skill )
	if self == character then
		return skill:canCastToSelf()
	elseif self:isLeagueWith(character) then
		return skill:canCastToLeague()
	else
		return skill:canCastToEnemty()
	end
end

function WarriorBarrier:setTile( tile ) 
	if base.setTile(self, tile) then
		local translate = tile:getCenterVector()
		self.object:resetTranslate(translate)
		g_dispatcher:dispatch("Warrior.settle", tile, self.object)
	end
end

return WarriorBarrier
