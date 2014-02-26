--- 
-- 对AI进行控制和切换的Manager
-- @module AIManager
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CharacterManager = require "CharacterManager"


local AIManager = CharacterManager.new{team = "AI"}

function AIManager:init( AIs )
	-- init heroes on map
	for i,AI in ipairs(AIs) do
		local character = 
			{
				career = "WARRIOR",

				level = 1,
				currentExp = 0,
				getExp = 0,
				
				name = "Tom" .. i,
				skills = { 1, 3 }

			}
		character.name = AI.name
		self.currentCharacter = self:createCharacter(character, AI.y, AI.x)
		self.currentCharacter:appendComponent(c"Actor", {manager = self})
	end
end

function AIManager:runActors(  )
	for characterObject in scene:objectsWithTag(self.team) do
		local actor = characterObject:findComponent(c"Actor")
		actor:run(coroutine.running())
		coroutine.yield()
	end
end

return AIManager