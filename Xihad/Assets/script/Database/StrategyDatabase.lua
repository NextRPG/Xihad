--- 
-- 存储各职业AI的职业属性，职业影响到了AI的进攻策略
-- @module name
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local StrategyDatabase = {

}

function loadSreategy( strategy )
	StrategyDatabase[strategy.id] = strategy
end

function StrategyDatabase:createStrategy( id, param )
	if param == nil then
		return table.copy(self[id])
	else
		return table.merge(self[id], param)
	end
end

loadSreategy{
	id = 1,
	name = "BaseStrategy",
	-- judgeTile
	distanceFactor = 0.5,
	HPFactor = 0.5,
	-- judgeSkill
	damageFactor = 0.3,
	currentTimeFactor = 0.3,
	rangeFactor = 0.4
}

loadSreategy{
	id = 2,
	name = "RemoteStrategy",
	-- judgeTile
	distanceFactor = 0.5,
	HPFactor = 0.5,
	-- judgeSkill
	damageFactor = 0.3,
	currentTimeFactor = 0.3,
	rangeFactor = 0.4
}

return StrategyDatabase