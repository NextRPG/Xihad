local CharacterManager = require "CharacterManager"
local CameraManager = require "CameraManager"

local HeroManager = CharacterManager.new{
	team = "Hero", 
	roundStart_aux = CharacterManager.roundStart
}

function HeroManager:init(heros)
	-- init heroes on map
	for i,hero in ipairs(heros) do
		local character = 
			{
				career = "WARRIOR",

				level = 1,
				currentExp = 0,
				getExp = 0,
				
				name = "Tom" .. i,
				skills = { 1, 2, 3 },
				properties = {
					physicalAttack = 5,
					physicalDefense = 5,
					magicAttack = 5,
					magicDefense = 10,  
					maxAP = 6,			
					maxHP = 200
				}

			}
		character.name = hero.name
		self.currentCharacter = self:createCharacter(character, hero.y, hero.x)
	end
end

function HeroManager:roundStart(  )
	CameraManager:move2Character(self.currentCharacter)
	self:roundStart_aux(  )
end

return HeroManager