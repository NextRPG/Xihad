--- 
-- 控制AI进行动作的控制协程
-- @module Actor
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local PathFinder = require "PathFinder"
-- local BaseStrategy = require "BaseStrategy"
local Chessboard = require "Chessboard"
local SkillManager = require "SkillManager"
local CameraManager = require "CameraManager"

---
-- @thread cothread
-- @tparam Strategy strategy
local Actor = {
	
}

function Actor.new( o, object )
	o = o or {}
	setmetatable(o, {__index = Actor})
	assert(o.manager)

	o.strategy.object = object
	o.strategy = require(o.strategy.name).new(o.strategy)
	-- o.strategy = BaseStrategy.new{object = object}
	return o
end

---
-- cothread从上次中断的地方返回继续执行
-- @... 传入任意长参数...
function Actor:run( scheduler )
	cothread = coroutine.create(function ( scheduler )
		local object    = self.object
		local strategy  = self.strategy
		local manager   = self.manager			
		local character = object:findComponent(c"Character")
		local tile      = character.tile

		manager.currentCharacter = object

		CameraManager:move2Character(object)

		local point, finder = strategy:judgeTile()
		finder = finder or require("GoalFinder")
		-- runAsync
		if not math.p_same(point, character.tile) then 
			manager:onSelectTile(Chessboard:tileAt(point), finder)
		end

		local selectSkill, target = strategy:judgeSkill() -- component
		if selectSkill ~= 0 then
			SkillManager:onCastSkill( target, selectSkill, character)
		end

		runCallback(coroutine.resume(scheduler)) 

		scheduler = coroutine.yield()
	end)
	runCallback(coroutine.resume( cothread, scheduler ))
end


return Actor