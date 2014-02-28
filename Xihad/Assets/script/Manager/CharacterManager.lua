--- 
-- 负责CharacterObject的创建，查询，和控制		
-- @module ChracterManager
-- @autor wangxuanyi
-- @license MIT
-- @copyright NextRPG

local PathFinder = require "PathFinder"
local Chessboard = require "Chessboard"
local CameraManager = require "CameraManager"
local SkillManager = require("SkillManager")

---
-- 记录当前team信息的容器
-- @string team 标识manager的队伍信息
-- @tparam Object currentCharacter
local CharacterManager = {
	team = ""
}

function CharacterManager.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = CharacterManager})

	return o
end

---
-- 创建CharacterObject
-- @tparam Character character
-- @int i 初始纵坐标 
-- @int j 初始横坐标
-- @treturn Object characterObject
function CharacterManager:createCharacter( character, i, j )
	assert(character.name)
	character.model = "ninja"
	character.team = self.team
	local characterObject = scene:createObject(c(character.name))

	local test = characterObject:appendComponent(c"Character", character)
	assert(test)
	local tile = {y = i, x = j}

	local location = getPixelLocation(tile)
	characterObject:concatTranslate(math3d.vector(location.x, 0, location.z))

	local param = { 
		mesh  = "Assets/model/".. character.model .. "/" .. character.model ..  ".b3d", 
		clips = "Assets/model/".. character.model .. "/" .. character.model ..  ".clip" 
	}
	local anim = characterObject:appendComponent(c"AnimatedMesh", param)
	anim:createSelector(c"stupid") 
	anim:playAnimation(c"idle 1")

	characterObject:appendComponent(c"MoveBy")
	characterObject:appendComponent(c"RotateBy")
	characterObject:appendComponent(c"Sequence")

	characterObject:addTag(c"Character")
	characterObject:addTag(c(self.team))

	for i,v in ipairs(character.skills) do
		SkillManager:createSkill(v)
	end

	characterObject:concatTranslate(math3d.vector(0, 3, 0))
	characterObject:resetScale(math3d.vector(1.5, 1.5, 1.5))
	-- characterObject:resetScale(math3d.vector(8, 8, 8))

	return characterObject	
end

--- 
-- 通过位置得到CharacterObject
-- @tab location
-- @treturn Object characterObject or nil
function CharacterManager:getCharacterByLocation( point )
	for characterObject in scene:objectsWithTag(self.team) do
		local character = characterObject:findComponent(c"Character")
		if math.p_same(character:tile(), point) then
		 	return characterObject:findComponent(c"Character")
		end
	end
	return nil
end

--- 
-- 选中角色后的行为
-- @tparam Object characterObject
function CharacterManager:onSelectCharacter( object )
	self.currentCharacter = object
	local character = object:findComponent(c"Character")
	PathFinder:getReachableTiles(character:tile(),character:getProperty("maxAP"))
	CameraManager:move2Character(object)
	Chessboard:pushArea(PathFinder, "BLUE")
	Chessboard:pushArea(
		SkillManager:getAllAvailableTargets(
			self.currentCharacter:findComponent(c"Character")), "RED")
end

local function optimizePath( actions )
	local newActions = {}
	newActions[1] = actions[1]
	for i,action in ipairs(actions) do
		if actions[i + 1] ~= nil then
			if table.equal(action, actions[i + 1]) then
				newActions[#newActions].destination = math.p_add(newActions[#newActions].destination, actions[i + 1].destination)
				newActions[#newActions].interval = newActions[#newActions].interval + actions[i + 1].interval
			else
				newActions[#newActions + 1] = actions[i + 1]
			end
		end
	end
	return newActions
end

---
-- 选中要走的路径之后的行为
-- @tparam Object characterObject
local lastTranslate = math3d.vector(0, 0, 0)
function CharacterManager:onSelectTile( tile, finder )

	Chessboard:pushArea(PathFinder, "ALPHA")
	Chessboard:pushArea(SkillManager.allTargets, "ALPHA")

	lastTranslate = self.currentCharacter:getTranslate()
	finder = finder or PathFinder 	
	local path = finder:constructPath(tile)

	local directions = Consts.directions
	local sequence = self.currentCharacter:findComponent(c"Sequence")
	-- TODO：优化路径
	local actions = {}
	for i,v in ipairs(path) do
		actions[#actions + 1] = {destination = directions[v], interval = 0.2}
	end
	actions = optimizePath(actions)

	local follow = CameraManager.camera:findComponent(c"CameraFollow")
	follow:start(self.currentCharacter)
	runAsyncFunc(sequence.runMoveActions, sequence, actions)
	follow:stop()
end

function CharacterManager:back2ShowCharacter(  )
	self.currentCharacter:resetTranslate(lastTranslate)
	Chessboard:popArea(PathFinder)
	Chessboard:popArea(SkillManager.allTargets)
	CameraManager:move2Character(self.currentCharacter)
end



---
-- 回合开始时，进行初始化工作
function CharacterManager:roundStart(  )
	-- handle message that round on Character has started
	for characterObject in scene:objectsWithTag(self.team) do
		local character = characterObject:findComponent(c"Character")
		character.states.TURNOVER = false
	end
end

---
-- 检查该人物是否被选过了
-- @tparam Object characterObject
function CharacterManager:checkAvailable( object )
	local states = object:findComponent(c"Character").states
	if 	states.TURNOVER == false  then
		return true
	end
	return false
end


function CharacterManager:checkRoundOver(  )
	for characterObject in scene:objectsWithTag(self.team) do
		if self:checkAvailable(characterObject) then
			return false
		end
	end
	return true
end

return CharacterManager