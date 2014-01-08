local CharacterManager = require "CharacterManager"

local HeroManager = CharacterManager.new{team = "Hero"}

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
				skills = { 1, 2, 3 }

			}
		character.name = hero.name
		self.currentCharacter = self:createCharacter(character, hero.y, hero.x)
	end
end

return HeroManager