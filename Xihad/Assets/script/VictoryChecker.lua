--- 
-- 检查战斗是否胜利
-- @module VictoryChecker
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG


local VictoryChecker = {

}

function VictoryChecker:init(  )
	
end

-- function VictoryChecker:on(  )
	
-- end

function VictoryChecker:clearEnemies(  )
	if next(scene:objectsWithTag("AI")) == nil then
		return true
	end
	return false
end

function VictoryChecker:clearHeroes(  )
	if next(scene:objectsWithTag("Hero")) == nil then
		return true
	end
	return false
end

function VictoryChecker:( ... )
	-- body
end

return VictoryChecker