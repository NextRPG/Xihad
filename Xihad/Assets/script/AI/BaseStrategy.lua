local GoalFinder = require "GoalFinder"
local SkillManager = require "SkillManager"
local ScoreBoard = require "ScoreBoard"
local Chessboard = require "Chessboard"

local BaseStrategy = {

}

function BaseStrategy.new( o )
	assert(type(o) == "table", "prototype RandomStratedy must be a table")
	setmetatable(o, {__index = BaseStrategy})

	return o
end

function BaseStrategy:judgeTile(  )
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

function BaseStrategy:judgeSkill(  )
	local skills = self.object:findComponent(c"Character").skills
	local center = self.object:findComponent(c"Character"):tile()

	local names, damages, currentTimes, ranges = {},{},{},{}
	for i,id in ipairs(skills) do
		repeat
			local skill = SkillManager:getSkill(id):findComponent(c"Skill")
			if not skill:hasEnemy( center ) then break end			
			names[#names + 1] = skill.id
			damages[skill.id] = skill.damage
			currentTimes[skill.id] = skill.currentTimes
			ranges[skill.id] = #skill.range
		until true
	end	

	if #names == 0 then
		return 0
	end

	local board = ScoreBoard.new{}

	board:appendKey( names )
	board:appendValue( damages, 0.3 )
	board:appendValue( currentTimes, 0.3 )
	board:appendValue( ranges, 0.4 )

	local selectSkill = SkillManager:getSkill(board:getResult()):findComponent(c("Skill"))
	local target = selectSkill:getBestTarget(center)

	return selectSkill, Chessboard:tileAt(target)
end

return BaseStrategy