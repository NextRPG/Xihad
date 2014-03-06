--- 
-- 为拥有远程攻击技能的敌人设计的策略
-- @module RemoteStrategy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local BaseStrategy = require "BaseStrategy"
local Scoreboard = require "Scoreboard"
local GoalFinder = require "GoalFinder"

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
	local name = self:judgePerson(  )
	print("attacking " .. name)
	local enemy = scene:findObject(c(name)):findComponent(c"Character")
	local tile = GoalFinder:getTargetTileRemote( actor.tile, enemy.tile, actor:getProperty("maxAP"), 2)
	return tile
end

return RemoteStrategy