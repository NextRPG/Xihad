require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"

g_scene:requireSystem(c"Audio")
g_audioSystem:setVolume(0)

local param = { mesh  = "Assets/model/ninja.b3d", clips = "Assets/model/ninja.clip" }
local ninja = g_scene:createObject(c"ninja")
ninja:resetTranslation(math3d.vector(0, 0, -15))
anim = ninja:appendComponent(c"AnimatedMesh", param)
anim:getMaterial(0):setColorMaterial('none')
anim:getMaterial(0):setDiffuseColor(Color.new(Color.black))

anim:playAnimation(c"idle 1")
anim:setTransitionTime(0.1)
ninja:concatRotate(math3d.vector(0, 180, 0))

local ctrl = ninja:appendComponent(c'ControlledMove')
local njaudio = ninja:appendComponent(c'Audio')
njaudio:play3D("Assets/mfx/bbb.ogg")
njaudio:setLooped(true)
ctrl:acquire()	-- control ninja

local cameraObject = g_scene:createObject(c"camera")
local camera = cameraObject:appendComponent(c"Camera")
cameraObject:concatTranslate(math3d.vector(0, 8, -25))

----- BGM
local root = g_scene:findObject(c"root")
local bgmPlayer = root:appendComponent(c"Audio")
bgmPlayer:play2D("Assets/mfx/dance.ogg")
bgmPlayer:setVolume(0.5)

root:appendUpdateHandler({ 
	onUpdate = function(self)
	if g_time.global >= 6 then
		self:stop()
	elseif g_time.global >= 5 then
		g_audioSystem:setVolume(6-g_time.global)
	elseif g_time.global >= 2.3 then
		if not self.playedTwice then
			bgmPlayer:play3D("Assets/mfx/appear.ogg")
			self.playedTwice = true
		end
	elseif g_time.global >= 2 then
		if not self.played then
			bgmPlayer:play3D("Assets/mfx/appear.ogg")
			self.played = true
		end
	elseif g_time.global < 1 then
		g_audioSystem:setVolume(g_time.global)
	end
end})

local position = cameraObject:getTranslation()
g_audioSystem:setListenerPosition(
	position,
	camera:getTarget()-position,
	camera:getUpVector())

