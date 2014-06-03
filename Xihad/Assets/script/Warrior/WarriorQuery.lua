local functional = require 'std.functional'
local WarriorQuery = {}

local function warriorIter(s, v)
	local object, warrior
	repeat
		object = s.f(s.s, sv)
		if not object then break end
		warrior= object:findComponent(c'Warrior')
	until warrior
		
	return warrior
end

function WarriorQuery.withTag(tag)
	local f, s, v = g_scene:objectsWithTag(tag)
	local iterData = { f = f, s = s }
	return functional.bind3(functional.return_, warriorIter, iterData, v)
end

local function getMinCost(warrior, location, minCost, minLocation)
	local path, cost = g_chessboard:routeRev(warrior, location)
	if not path then
		return minCost, minLocation
	elseif not minCost then
		return cost, location
	elseif cost < minCost then
		return cost, location
	else
		return minCost, minLocation
	end
end

function WarriorQuery.minCostToApproach(warrior, enemy)
	local target = enemy:getLocation()
	local cost, location
	cost, location = getMinCost(warrior, target:top(),  cost, location)
	cost, location = getMinCost(warrior, target:down(), cost, location)
	cost, location = getMinCost(warrior, target:left(), cost, location)
	cost, location = getMinCost(warrior, target:right(),cost, location)
	
	return cost, location
end

return WarriorQuery
