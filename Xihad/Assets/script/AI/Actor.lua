--- 
-- 控制AI进行动作的控制协程
-- @module Actor
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local PathFinder = require "PathFinder"
local BaseStrategy = require "BaseStrategy"
local Chessboard = require "Chessboard"
local SkillManager = require "SkillManager"

---
-- @thread cothread
-- @tparam Strategy strategy
local Actor = {
	
}

function Actor.new( o, object )
	o = o or {}
	setmetatable(o, {__index = Actor})
	assert(o.manager)

	o.strategy = BaseStrategy.new{object = object}
	o:initThread(  )
	return o
end

---
-- cothread从上次中断的地方返回继续执行
-- @... 传入任意长参数...
function Actor:run( ... )
	runCallback(coroutine.resume( self.cothread, ... ))
end

function Actor:initThread(  )
	self.cothread = coroutine.create(function ( scheduler )
		while true do
			local object    = self.object
			local strategy  = self.strategy
			local manager   = self.manager			
			local character = object:findComponent(c"Character")
			local tile      = character:tile()

			manager.currentCharacter = object

			local point = strategy:judgeTile()
			-- runAsync
			manager:onSelectTile(Chessboard:tileAt(point), require("GoalFinder"))
			
			local selectSkill, target = strategy:judgeSkill() -- component
			if selectSkill ~= 0 then
				print(" the skill is ", selectSkill)
				SkillManager:onCastSkill( target, selectSkill, object)
			end

			coroutine.resume(scheduler)

			scheduler = coroutine.yield()
		end
	end)
end


return Actor