--- 
-- 为拥有远程攻击技能的敌人设计的策略
-- @module RemoteStrategy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local BaseStrategy = require "BaseStrategy"
local Scoreboard = require "Scoreboard"

local RemoteStrategy = {
	judgeTile_aux = BaseStrategy.judgeTile,
	judgeSkill_aux = BaseStrategy.judgeSkill
}

function RemoteStrategy.new( o )
	assert(type(o) == "table", "prototype must be a table")
	inherit(o, RemoteStrategy)

	return o
end

function RemoteStrategy:judgeTile(  )
	
end

function RemoteStrategy:judgeSkill(  )
	
end

return RemoteStrategy