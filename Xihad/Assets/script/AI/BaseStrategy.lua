local GoalFinder = require "GoalFinder"
local SkillManager = require "SkillManager"
local ScoreBoard = require "ScoreBoard"
local Chessboard = require "Chessboard"

local BaseStrategy = {
	object = nil
}

function BaseStrategy.new( o )
	assert(type(o) == "table", "prototype RandomStratedy must be a table")
	setmetatable(o, {__index = BaseStrategy})
	
	return o
end

function BaseStrategy:judgePerson(  )
	local actor = self.object:findComponent(c"Character")
	local names, distances, HPs = {}, {}, {}

	for object in scene:objectsWithTag("Hero") do
		repeat
			local enemy = object:findComponent(c"Character")
			local costAP = GoalFinder:getCostAP(actor.tile ,enemy.tile, actor:getProperty("maxAP"))
			if costAP == "MAX" then break end
			names[#names + 1] = object:getID()
			distances[object:getID()] = costAP
			HPs[object:getID()] = - enemy:getProperty("currentHP")
		until true
	end	

	local board = ScoreBoard.new{}

	board:appendKey( names )
	board:appendValue( distances, self.distanceFactor )
	board:appendValue( HPs, self.HPFactor )

	local name = board:getResult()
	return name
end

function BaseStrategy:judgeTile(  )
	local name = self:judgePerson(  )
	local enemy = scene:findObject(c(name)):findComponent(c"Character")
	local tile = GoalFinder:getTargetTile( actor.tile, enemy.tile, actor:getProperty("maxAP"))
	
	return tile
end

function BaseStrategy:judgeSkill(  )
	local character = self.object:findComponent(c"Character")
	local skills = character.skills
	local center = character.tile

	local names, damages, consumeMPs, ranges = {},{},{},{}
	for i,id in ipairs(skills) do
		repeat
			local skill = SkillManager:getSkill(id)

			if not character:canTrigger(skill) or
			   not skill:hasEnemy( center, character:getEnemyManager())
			then break end		

			names[#names + 1] = skill.id
			damages[skill.id] = skill.damage
			consumeMPs[skill.id] = skill.consumeMP
			ranges[skill.id] = #skill.range
		until true
	end	

	if #names == 0 then
		return 0
	end

	local board = ScoreBoard.new{}

	board:appendKey( names )
	board:appendValue( damages, self.damageFactor )
	board:appendValue( consumeMPs, self.consumeMPFactor )
	board:appendValue( ranges, self.rangeFactor )

	local selectSkill = SkillManager:getSkill(board:getResult())
	local target = selectSkill:getBestTarget(center)

	return selectSkill, Chessboard:tileAt(target)
end

return BaseStrategy