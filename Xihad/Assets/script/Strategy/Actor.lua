--- 
-- 控制AI进行动作的控制协程
-- @module Actor
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Chessboard = require "ColoringManager"
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
function Actor:run()
	cothread = coroutine.create(function ()
		local object    = self.object
		local strategy  = self.strategy
		local manager   = self.manager			
		local character = object:findComponent(c"Character")
		local tile      = character.tile

		manager.currentCharacter = object

		CameraManager:back2Normal(object)
		local point, finder = strategy:judgeTile()
		-- runAsync
		if not math.p_same(point, character.tile) then 
			local tile = Chessboard:tileAt(point)
			local path = finder:constructPath(tile)
			manager:onSelectTile(tile, path)
		end
		
		local selectSkill, target = strategy:judgeSkill() -- component
		if selectSkill ~= 0 then
			SkillManager:onCastSkill( target, selectSkill, character)
		end
		
		character:onRoundOver()
	end)
	
	coroutine.resume(cothread)
end


return Actor