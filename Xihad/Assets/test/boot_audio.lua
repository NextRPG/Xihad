require "math3d"

local param = { 
	mesh  = "Assets/model/ninja.b3d", 
	clips = "Assets/model/ninja.clip" 
}

local ninja = scene:createObject(c"ninja")
do
	anim = ninja:appendComponent(c"AnimatedMesh", param)
	anim:createSelector(c"stupid")
end
anim:playAnimation(c"idle 1")
anim:setTransitionTime(0.1)
ninja:concatRotate(math3d.vector(0, 180, 0))

local cameraObject = scene:createObject(c"camera")
local camera = cameraObject:appendComponent(c"Camera")
cameraObject:concatTranslate(math3d.vector(0, 8, -25))




----- BGM
-- local root = scene:findObject(c"__ROOT__")
-- local bgmPlayer = root:appendComponent(c"Audio")
-- bgmPlayer:playMusic("Assets/mfx/bbb.ogg")

-- root:appendUpdateHandler({ 
-- 	onUpdate = function(self)
-- 	if Time.global >= 5 then
-- 		-- bgmPlayer:stopAudio()
-- 		bgmPlayer:playSound("Assets/mfx/appear.ogg")
-- 		self:stop()
-- 	end
-- end})
