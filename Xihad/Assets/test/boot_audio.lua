require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"

scene:requireSystem(c"Audio")
audioSystem:setVolume(0)

local param = { mesh  = "Assets/model/ninja.b3d", clips = "Assets/model/ninja.clip" }
local ninja = scene:createObject(c"ninja")
ninja:resetTranslation(math3d.vector(0, 0, -15))
anim = ninja:appendComponent(c"AnimatedMesh", param)
anim:playAnimation(c"idle 1")
anim:setTransitionTime(0.1)
ninja:concatRotate(math3d.vector(0, 180, 0))

local ctrl = ninja:appendComponent(c'ControlledMove')
local njaudio = ninja:appendComponent(c'Audio')
njaudio:play3D("Assets/mfx/bbb.ogg")
njaudio:setLooped(true)
ctrl:acquire()	-- control ninja

local cameraObject = scene:createObject(c"camera")
local camera = cameraObject:appendComponent(c"Camera")
cameraObject:concatTranslate(math3d.vector(0, 8, -25))

----- BGM
local root = scene:findObject(c"root")
local bgmPlayer = root:appendComponent(c"Audio")
bgmPlayer:play2D("Assets/mfx/dance.ogg")
bgmPlayer:setVolume(0.5)

root:appendUpdateHandler({ 
	onUpdate = function(self)
	if Time.global >= 6 then
		self:stop()
	elseif Time.global >= 5 then
		audioSystem:setVolume(6-Time.global)
	elseif Time.global >= 2.3 then
		if not self.playedTwice then
			bgmPlayer:play3D("Assets/mfx/appear.ogg")
			self.playedTwice = true
		end
	elseif Time.global >= 2 then
		if not self.played then
			bgmPlayer:play3D("Assets/mfx/appear.ogg")
			self.played = true
		end
	elseif Time.global < 1 then
		audioSystem:setVolume(Time.global)
	end
end})

local position = cameraObject:getTranslation()
audioSystem:setListenerPosition(
	position,
	camera:getTarget()-position,
	camera:getUpVector())
