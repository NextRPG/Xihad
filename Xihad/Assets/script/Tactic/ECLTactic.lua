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
local Location = require 'route.Location'
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

local function encode(skill, impactCenter)
	return skill:getName()..'@'..tostring(impactCenter)
end

local function decode(castID)
	local f, s, var = string.gmatch(castID, '[^@]+')
	local skillName = f(s, var)
	local locationStr = f(s, skillName)
	
	f, s, var = string.gmatch(locationStr, '[%d]+')
	local x = f(s, var)
	local y = f(s, x)
	return skillName, Location.new(tonumber(x), tonumber(y))
end

local function insert(map, enemy, skill, impactCenter, launcherLoc)
	local castID = encode(skill, impactCenter)
	
	local toEnemy = map[enemy] or {}
	local castToEnemy = toEnemy[castID] or {}
	table.insert(castToEnemy, launcherLoc)
	
	-- write back
	toEnemy[castID] = castToEnemy
	map[enemy] = toEnemy
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
	local tiles = skill:getImpactTiles(g_chessboard, centerLoc)
	for _, impactTile in ipairs(tiles) do
		local affectWarrior = self:_get_enemy(impactTile, warrior)
		if affectWarrior then
			insert(map, affectWarrior, skill, centerLoc, launcherLoc)
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

function ECLTactic:_build_mapping(warrior, reachables)
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
		functional.bind2(self.castGrader, warrior, bestEnemy))
	
	if not bestCast then return bestEnemy end
	
	bestLocation = GradeSelector.select(
		functional.bind1(Array.elements, map[bestEnemy][bestCast]),
		functional.bind3(self.standGrader, warrior, bestEnemy, bestCast))
	
	return bestEnemy, bestCast, bestLocation
end

function ECLTactic:_try_the_best_to_approach(warrior, location)
	-- This ensures luaT::checkArg<Component*>() cast delegate to Component*
	local mt = setmetatable({}, getmetatable(warrior))
	
	-- This ensures delegate copy function and field from warrior
	mt.__index = warrior
	
	-- Take all in effect
	local delegate = setmetatable({}, mt)
	
	-- Overerite this function to ignore every enemy to reach the location
	delegate.isLeagueWith = functional.bind1(functional.return_, true)
	
	return g_chessboard:routeRev(delegate, location)
end

function ECLTactic:_select_approaching(warrior, bestEnemy, reachables)
	local standing = warrior:getLocation()
	if not self.approaching then
		return standing
	end
	
	local expectedLocation = self.approaching(warrior, bestEnemy)
	if not expectedLocation then
		return standing
	end
	
	local revPath, _ = g_chessboard:routeRev(warrior, expectedLocation)
	if not revPath then
		revPath = self:_try_the_best_to_approach(warrior, expectedLocation)
	end
	
	if not revPath then
		return standing -- can not reach expected tile
	end
	
	for _, location in ipairs(revPath) do
		local tile = g_chessboard:getTile(location)
		if reachables[tile] then
			return location
		end
	end
	
	return standing
end

function ECLTactic:_get_reachables(warrior)
	local array = g_chessboard:getReachableTiles(warrior)
	local set = {}
	for _, e in ipairs(array) do
		set[e] = true
	end
	
	return set
end

function ECLTactic:_completeOrder(cmdList)
	local warrior = cmdList:getSource()
	local reachables = self:_get_reachables(warrior)
	local map = self:_build_mapping(warrior, reachables)
	
	local enemy, cast, location = self:_select_best(warrior, map)
	if not location then
		location = self:_select_approaching(warrior, enemy, reachables)
		
		cmdList:setCommand('待机')
	else
		assert(enemy ~= nil and cast ~= nil)
		local cmd, targetLoc = decode(cast)
		cmdList:setCommand('技能', cmd)
		cmdList:setTarget(targetLoc)
	end
	
	assert(location ~= nil)
	cmdList:setLocation(location)
end

return ECLTactic
