--- 
-- AI的的策略模型的基类
-- @module Strategy
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local Chessboard = require "Chessboard"

---
-- @tparam Object CharacterObject object
local Strategy = {
	object = nil
}

function Strategy.new( o )
	assert(type(o) == "table", "prototype must be a table")
	assert(o.object, "strategy must have its object")
	setmetatable(o, {__index = Strategy})

	return o
end

---
-- @float 距离远近因素
-- @float 人物血量因素
-- @treturn Tile tile 
function Strategy:chooseTile( near, weak,  )
	
end

function Strategy:judgeTile(  )
	local character = self.object:findComponent(c"Character")
	for i, p in ipairs(PathFinder) do
		if not table.equal(p , character:tile()) then
			return p
		end
	end
end

function Strategy:judgeSkill(  )
	local skills = self.object:findComponent(c"Character").skills
	for i,id in ipairs(skills) do
		local skill = SkillManager:getSkill(id):findComponent(c"Skill")
		if skill:
	end	
end

function Strategy:judgeTarget(  )
	
end

return Strategy