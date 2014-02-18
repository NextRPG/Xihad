local PathFinder = require "PathFinder"
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
	PathFinder:getAllReachableTiles(character:tile())
	print("haha")
	for hero in scene:objectsWithTag("Hero") do
		names[#names + 1] = hero:getID()

		local temp = hero:findComponent(c"Character")
		
		distances[#distances + 1] = PathFinder:getCostAP(temp:tile())

		HPs[#HPs + 1] = - temp:getProperty("currentHP")
	end
	local board = ScoreBoard.new{}
	board:appendKey( names )
	board:appendValue( distances, 1 )
	board:appendValue( HPs, 0 )
	local name = board:getResult()
	character = scene:findObject(c(name)):findComponent(c"Character")
	return PathFinder:getEnemyTile(character:tile(), character:getProperty("maxAP") )
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