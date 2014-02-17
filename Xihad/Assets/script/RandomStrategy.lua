local PathFinder = require "PathFinder"
local SkillManager = require "SkillManager"

local RandomStrategy = {

}

function RandomStrategy.new( o )
	assert(type(o) == "table", "prototype RandomStratedy must be a table")
	setmetatable(o, {__index = RandomStrategy})

	return o
end

function RandomStrategy:judgeTile(  )
	local character = self.object:findComponent(c"Character")
	for i, p in ipairs(PathFinder) do
		if not table.equal(p , character:tile()) then
			return p
		end
	end
end

function RandomStrategy:judgeSkill(  )
	local skills = self.object:findComponent(c"Character").skills
	for i,id in ipairs(skills) do
		local skill = SkillManager:getSkill(id):findComponent(c"Skill")
	end	
end

function RandomStrategy:judgeTarget(  )
	
end

return RandomStrategy