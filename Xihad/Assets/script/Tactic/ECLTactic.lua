---
-- Enemy-Cast-Location-Tactic
--
-- select enemies those are in attack range
-- 	-> EnemyGrader (enemy)
-- 		-> select all tile where can cast skill to attack the (enemy)
-- 			-> CastGrader (skill, impactCenter)
-- 				-> select all tile where the source can reach to cast 
--					(skill, impactCenter)
-- 					-> LocationGrader (location)
-- 	-> if failed, Approaching (approach)
-- 		-> move to the closet tile to (approach), and stand by
--
-- 进攻型、防御型、驻扎型、复仇型战术
--
local base = require 'Tactic'
local Array= require 'std.Array'
local Table= require 'std.Table'
local functional = require 'std.functional'
local GradeSelector = require 'Tactic.GradeSelector'

local ECLTactic = {
	enemyGrader = nil,
	castGrader  = nil,
	standGrader = nil,
	approaching = nil,
}
ECLTactic.__index= ECLTactic
ECLTactic.__base = 'Tactic'
setmetatable(ECLTactic, base)

function ECLTactic.new(graders)
	assert(graders.enemy and graders.cast and graders.location)
	
	local o = base.new()
	o.enemyGrader = graders.enemy
	o.castGrader  = graders.cast
	o.standGrader = graders.location
	o.approaching = graders.approaching
	
	return setmetatable(o, ECLTactic)
end

function ECLTactic:insert(map, enemy, skill, impactCenter, launcherLoc)
	local castID = self:_encode(skill, impactCenter)
	
	local toEnemy = Table.get_or_new_table(map, enemy)
	local castToEnemy = Table.get_or_new_table(toEnemy, castID)
	table.insert(castToEnemy, launcherLoc)
end

local log = function () end
-- local log = function(fmt, ...) io.write(string.format(fmt, ...)) end
function ECLTactic:_get_enemy(tile, warrior)
	if not tile:hasWarrior() then
		log('noWarrior\n')
		return
	end
	
	log('hasWarrior\t')
	local affectWarrior = tile:getWarrior()
	if warrior:isEnemyWith(affectWarrior) then
		log('isEnemy(%s, %s)\n', warrior:getTeam(), affectWarrior:getTeam())
		return affectWarrior
	else
		log('isNotEnemy\n')
	end
end

function ECLTactic:_cast_on_tile(warrior, launcherLoc, skill, centerLoc, map)
	local tiles = skill:getImpactTiles(g_chessboard, launcherLoc, centerLoc)
	for _, impactTile in ipairs(tiles) do
		local affectWarrior = self:_get_enemy(impactTile, warrior)
		if affectWarrior then
			self:insert(map, affectWarrior, skill, centerLoc, launcherLoc)
		end
	end
end

function ECLTactic:_test_skill(warrior, launcherLoc, skill, map)
	log('\t%s:\t', skill:getName())
	if not skill:canCastToEnemy() then
		log('canNotCastToEnemy\n')
		return
	end
	
	log('canCastToEnemy\n')
	local launchables = 
		skill:getLaunchableTiles(g_chessboard, warrior, launcherLoc)
		
	for centerTile, _ in pairs(launchables) do 
		local centerLoc = centerTile:getLocation()
		self:_cast_on_tile(warrior, launcherLoc, skill, centerLoc, map)
	end
end

function ECLTactic:_build_map(warrior, reachables)
	local skillCaster = warrior:findPeer(c'SkillCaster')
	
	log('ECLTactic for: %s\n', warrior:getHostObject():getID())
	local map = {}
	for tile, _ in pairs(reachables) do
		log('reach %s\t', tostring(tile:getLocation()))
		
		local launcherLoc = tile:getLocation()
		for skill in skillCaster:castableSkills() do
			self:_test_skill(warrior, launcherLoc, skill, map)
		end
	end
	
	return map
end

function ECLTactic:_select_best(warrior, map)
	local bestEnemy, bestCast, bestLocation
	
	bestEnemy = GradeSelector.select(
		functional.bind1(pairs, map), 
		functional.bind1(self.enemyGrader, warrior))
	
	if not bestEnemy then return end
	
	bestCast = GradeSelector.select(
		functional.bind1(pairs, map[bestEnemy]), 
		function (cast)
			return self.castGrader(warrior, bestEnemy, cast:decode())
		end)
	
	if not bestCast then return bestEnemy end
	
	bestLocation = GradeSelector.select(
		functional.bind1(Array.elements, map[bestEnemy][bestCast]),
		functional.bind3(self.standGrader, warrior, bestEnemy, bestCast))
	
	return bestEnemy, bestCast, bestLocation
end

function ECLTactic:_select_approaching(warrior, bestEnemy, reachables)
	local expected = nil
	if self.approaching then
		expected = self.approaching(warrior, bestEnemy)
	end
	
	return self:_approach(warrior, expected, reachables)
end

function ECLTactic:_completeOrder(warrior, reachables)
	return self:_select_best(warrior, self:_build_map(warrior, reachables))
end

return ECLTactic
