local PathFinder = require "PathFinder"
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
	for i, p in ipairs(PathFinder) do
		if not table.equal(p , character:tile()) then
			return p
		end
	end

	local actor = self.object:findComponent(c"Character")
	local names, distances, HPs = {}, {}, {}

	for object in scene:objectsWithTag("Hero") do
		names[#names + 1] = object:getID()
		local enemy = object:findComponent(c"Character")
		distances[object:getID()] = GoalFinder:getCostAP(actor:tile() ,enemy:tile())
		HPs[object:getID()] = - enemy:getProperty("currentHP")
	end	

	local board = ScoreBoard.new{}

	board:appendKey( names )
	board:appendValue( distances, 0.1 )
	board:appendValue( HPs, 0.9 )

	local name = board:getResult()
	local enemy = scene:findObject(c(name)):findComponent(c"Character")
	local tile = GoalFinder:getTargetTile( actor:tile(), enemy:tile(), actor:getProperty("maxAP"))
	return tile
end

function RandomStrategy:judgeSkill(  )
	local skills = self.object:findComponent(c"Character").skills
	for i,id in ipairs(skills) do
		local skill = SkillManager:getSkill(id):findComponent(c"Skill")
		if skill:

	end	
end

function RandomStrategy:judgeTarget(  )
	
end

return RandomStrategylocal PathFinder = require "PathFinder"
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
	for i, p in ipairs(PathFinder) do
		if not table.equal(p , character:tile()) then
			return p
		end
	end

	local actor = self.object:findComponent(c"Character")
	local names, distances, HPs = {}, {}, {}

	for object in scene:objectsWithTag("Hero") do
		names[#names + 1] = object:getID()
		local enemy = object:findComponent(c"Character")
		distances[object:getID()] = GoalFinder:getCostAP(actor:tile() ,enemy:tile())
		HPs[object:getID()] = - enemy:getProperty("currentHP")
	end	

	local board = ScoreBoard.new{}

	board:appendKey( names )
	board:appendValue( distances, 0.1 )
	board:appendValue( HPs, 0.9 )

	local name = board:getResult()
	local enemy = scene:findObject(c(name)):findComponent(c"Character")
	local tile = GoalFinder:getTargetTile( actor:tile(), enemy:tile(), actor:getProperty("maxAP"))
	return tile
end

function RandomStrategy:judgeSkill(  )
	local skills = self.object:findComponent(c"Character").skills
	for i,id in ipairs(skills) do
		local skill = SkillManager:getSkill(id):findComponent(c"Skill")
		if skill:

	end	
end

function RandomStrategy:judgeTarget(  )
	
end

return RandomStrategy