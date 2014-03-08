local CharacterManager = require "CharacterManager"
local CameraManager = require "CameraManager"

local HeroManager = CharacterManager.new{
	team = "Hero", 
	roundStart_aux = CharacterManager.roundStart
}

function HeroManager:init(points, heros)
	-- init heroes on map
	self.characters = {}
	for i,point in ipairs(points) do
		self.currentCharacter = self:createCharacter(heros[i], point.y, point.x)
	end
end

function HeroManager:roundStart(  )
	local object = scene:getObjectWithTag("Hero")
	CameraManager:move2Character(object)
	self:roundStart_aux(  )
end

return HeroManager