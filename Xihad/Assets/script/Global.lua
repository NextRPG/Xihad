Global = {
-- cosnts
}

package.loaded["Global"] = Global

local Chessboard = require "Chessboard"
local HeroManager = require "HeroManager"
local AIManager = require "AIManager"
require "ElementManager"
local CameraManager = require "CameraManager"
local BattleManager = require "BattleManager"

-- load resources
require "SkillDatabase"
require "TerrainDatabase"
require "Consts"



function Global.initSystem( ... )
	Chessboard:init()
	HeroManager:init()
	AIManager:init()
	BattleManager:init(HeroManager, AIManager)

	-- AIManager:init()

	-- elementManager = ElementManager:new()
	-- elementManager:init()

	-- chessboard = Chessboard:new()
	-- chessboard:init()

	CameraManager:init()
end