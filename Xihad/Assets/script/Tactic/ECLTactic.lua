---
-- Enemy-Cast-Location-Tactic
--
-- select enemies those are in attack range
-- 	-> EnemyGrader (enemy)
-- 		-> select all tile where can cast skill to attack the (enemy)
-- 			-> CastGrader (skill, impactCenter)
-- 				-> select all tile where the source can reach to cast (skill, impactCenter)
-- 					-> LocationGrader (location)
-- 	-> if failed, Approaching (approach)
-- 		-> move to the closet tile to (approach), and stand by
--
-- 进攻型、防御型、驻扎型、复仇型战术
--

local base = require 'Tactic'
local Location = require 'route.Location'

local ECLTactic = {
	enemyGrader = nil,
	castGrader  = nil,
	locationGrader = nil,
	approaching = nil,
}
ECLTactic.__index = ECLTactic
ECLTactic.__base = 'Tactic'
setmetatable(ECLTactic, base)

function ECLTactic.new(graders)
	assert(graders.enemy and graders.cast and graders.location)
	
	local o = base.new()
	o.enemyGrader = graders.enemy
	o.castGrader  = graders.cast
	o.locationGrader = graders.location
	o.approaching = graders.approaching
	
	return setmetatable(o, ECLTactic)
end

function encode(skill, impactCenter)
	return skill:getName()..'@'..tostring(impactCenter)
end

function decode(castID)
	local f, s, var = string.gmatch(castID, '[^@]+')
	local skillName = f(s, var)
	local locationStr = f(s, skillName)
	
	f, s, var = string.gmatch(locationStr, '[%d]+')
	local x = f(s, var)
	local y = f(s, x)
	return skillName, Location.new(tonumber(x), tonumber(y))
end

local function insert(mapping, enemy, skill, impactCenter, launcherCenter)
	local castID = encode(skill, impactCenter)
	
	local toEnemy = mapping[enemy] or {}
	local castToEnemy = toEnemy[castID] or {}
	table.insert(castToEnemy, launcherCenter)
	
	-- write back
	toEnemy[castID] = castToEnemy
	mapping[enemy] = toEnemy
end


---
-- TODO FIXME 当角色处于右边界时，AI会攻击左边界
local log = function () end
-- local log = function(fmt, ...) io.write(string.format(fmt, ...)) end
function ECLTactic:_completeOrder(cmdList)
	local warrior = cmdList:getSource()
	local skillCaster = warrior:findPeer(c'SkillCaster')
	
	log('ECLTactic for: %s\n', warrior:getHostObject():getID())
	local mapping = {}
	local reachables = g_chessboard:getReachableTiles(warrior)
	for _, tile in ipairs(reachables) do
		log('reach %s\t', tostring(tile:getLocation()))
		
		for skill in skillCaster:castableSkills() do
		repeat -- let break as continue
			log('\t%s:\t', skill:getName())
			if not skill:canCastToEnemy() then
				log('canNotCastToEnemy\n')
				break	-- continue
			end
			
			log('canCastToEnemy\n')
			local range = skill:getRange()
			local launcherCenter = tile:getLocation()
			range:traverseLaunchableLocations(launcherCenter, function (impactCenterLoc)
				local impactCenterTile = g_chessboard:getTile(impactCenterLoc)
				log('\t\tcastCenter: %s\t', tostring(impactCenterLoc))
				
				if not impactCenterTile then 
					log('notExistTile\n')
					return 
				end
				log('existTile\t')
				
				if not impactCenterTile:permitCasting(warrior, skill) then
					log('notPermitCasting\n')
					return
				end
				log('permitCasting\n')
				
				range:traverseImpactLocations(impactCenterLoc, function (impactLocation)
					local impactTile = g_chessboard:getTile(impactLocation)
					log('\t\t\timpact: %s\t', tostring(impactLocation))
					if not impactTile or not impactTile:hasWarrior() then
						log('noWarrior\n')
						return
					end
					
					log('hasWarrior\t')
					local affectWarrior = impactTile:getWarrior()
					if warrior:isEnemyWith(affectWarrior) then
						log('isEnemy(%s, %s)\n', warrior:getTeam(), affectWarrior:getTeam())
						insert(mapping, affectWarrior, skill, impactCenterLoc, launcherCenter)
					else
						log('isNotEnemy\n')
					end
				end)
			end)
		until true
		end
	end
	
	local bestEnemy, bestCast, bestLocation
	local enemyHighScore = 0
	for enemy, _ in pairs(mapping) do
		local newScore, enough = self.enemyGrader(warrior, enemy)
		if enemyHighScore < newScore then
			enemyHighScore = newScore
			bestEnemy = enemy
		end
		
		if enough then break end
	end
	
	if bestEnemy then
		local castHighScore = 0
		for cast, _ in pairs(mapping[bestEnemy]) do
			local newScore, enough = self.castGrader(warrior, bestEnemy, cast)
			if castHighScore < newScore then
				castHighScore = newScore
				bestCast = cast
			end
			
			if enough then break end
		end
		
		if bestCast then
			local locationHighScore = 0
			for _, location in ipairs(mapping[bestEnemy][bestCast]) do
				local newScore, enough = self.locationGrader(warrior, bestEnemy, bestCast, location)
				if locationHighScore < newScore then
					locationHighScore = newScore
					bestLocation = location
				end
				
				if enough then break end
			end
		end
	end
	
	if not bestLocation then
		if self.approaching then
			local expectLocation = self.approaching(warrior)
			-- TODO find nearest reachable location to expectLocation
		else
			bestLocation = warrior:getLocation()
		end
		
		cmdList:setCommand('待机')
	else
		local cmd, targetLoc = decode(bestCast)
		cmdList:setCommand(cmd)
		cmdList:setTarget(targetLoc)
	end
	
	cmdList:setLocation(bestLocation)
end

return ECLTactic
