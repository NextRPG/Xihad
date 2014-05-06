--- 
-- 为拥有远程攻击技能的敌人设计的策略
-- @module RemoteStrategy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local BaseStrategy = require "BaseStrategy"
local Scoreboard = require "Scoreboard"
local GoalFinder = require "GoalFinder"
local PathFinder = require "PathFinder"

local RemoteStrategy = {
	-- judgeTile_aux = BaseStrategy.judgeTile,
	judgeSkill = BaseStrategy.judgeSkill,
	judgePerson = BaseStrategy.judgePerson
}

function RemoteStrategy.new( o )
	assert(type(o) == "table", "prototype must be a table")
	inherit(o, RemoteStrategy)

	return o
end

function RemoteStrategy:judgeTile(  )
	local actor = self.object:findComponent(c"Character")
	local name = self:judgePerson()
	local skills = actor:getSkills()
	local _, min = findMin(makeList(skills, "id", "minDistance"))
	local _, max = findMax(makeList(skills, "id", "maxDistance"))
	local distance = math.random(min, max + 1)
	local enemy = g_scene:findObject(c(name)):findComponent(c"Character")
	
	if math.p_distance(actor.tile, enemy.tile) > distance then
		local tile = GoalFinder:getTargetTileRemote( actor.tile, enemy.tile, actor:getProperty("maxAP"), distance)
		return tile, GoalFinder
	else
		local distances = {}
		actor:traverseReachableTiles(function (tile)
			distances[point] = math.p_distance(point, enemy.tile)
			if distances[point] >= distance then
				return point, PathFinder
			end
		end)

		-- PathFinder:getReachableTiles(actor)
		-- for i,point in ipairs(PathFinder) do
		-- 	distances[point] = math.p_distance(point, enemy.tile)
		-- 	if distances[point] >= distance then
		-- 		return point, PathFinder
		-- 	end
		-- end
		
		return findMax(distances), PathFinder
	end
end

return RemoteStrategy