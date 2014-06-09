local base = require 'Barrier'
local WarriorHitResult = require 'Warrior.WarriorHitResult'
local WarriorBarrier = {}
WarriorBarrier.__index = WarriorBarrier
WarriorBarrier.__base = 'Barrier'
setmetatable(WarriorBarrier, base)

function WarriorBarrier.new()
	return setmetatable(base.new(), WarriorBarrier)
end

function WarriorBarrier:newHitResult(source)
	return WarriorHitResult.new(source, self:findPeer(c'Warrior'))
end

function WarriorBarrier.getOptUniqueKey()
	return 'WarriorBarrier'
end

---
-- We should invoke other:isLeagueWith(self), not self:isLeagueWith(other), 
-- for support route cheat
function WarriorBarrier:isLeagueWith(other)
	return other:isLeagueWith(self:findPeer(c'Warrior'))
end

function WarriorBarrier:canPass( warrior )
	return self:isLeagueWith(warrior)
end

function WarriorBarrier:canStay( warrior )
	return warrior:getHostObject():getID() == self:getHostObject():getID()
end

function WarriorBarrier:permitCasting( warrior, skill )
	if self == warrior then
		return skill:canCastToSelf()
	elseif self:isLeagueWith(warrior) then
		return skill:canCastToLeague()
	else
		return skill:canCastToEnemy()
	end
end

-- function WarriorBarrier:setTile( tile ) 
-- 	if base.setTile(self, tile) then
-- 		if tile then
			
-- 		else
-- 			-- TODO
-- 		end
-- 	end
-- end

return WarriorBarrier
