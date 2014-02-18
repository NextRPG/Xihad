local GoalFinder = require "GoalFinder"
local SkillManager = require "SkillManager"
local ScoreBoard = require "ScoreBoard"

local RandomStrategy = {

}

function RandomStrategy.new( o )
	assert(type(o) == "table", "prototype RandomStratedy must be a table")
	setmetatable(o, {__index = RandomStrategy})

	return o
end

function RandomStrategy:judgeTile(  )

	local character = self.object:findComponent(c"Character")
	local names, distances, HPs = {}, {}, {}

	for hero in scene:objectsWithTag("Hero") do
		names[#names + 1] = hero:getID()
		local temp = hero:findComponent(c"Character")
		distances[hero:getID()] = GoalFinder:getCostAP(character:tile() ,temp:tile())
		HPs[hero:getID()] = - temp:getProperty("currentHP")
	end	

	local board = ScoreBoard.new{}

	board:appendKey( names )
	board:appendValue( distances, 0.1 )
	board:appendValue( HPs, 0.9 )

	local name = board:getResult()
	return scene:findObject(c(name)):findComponent(c"Character"):tile()
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