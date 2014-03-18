require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"

local param = { mesh  = "Assets/model/ninja.b3d", clips = "Assets/model/ninja.clip" }
local ninja = scene:createObject(c"ninja")
ninja:resetTranslation(math3d.vector(0, 0, -15))
anim = ninja:appendComponent(c"AnimatedMesh", param)
anim:playAnimation(c"idle 1")
anim:setTransitionTime(0.1)
ninja:concatRotate(math3d.vector(0, 180, 0))

local ctrl = ninja:appendComponent(c'ControlledMove')
ctrl:acquire()	-- control ninja

local njname = scene:createObject(c"ninjaName", ninja)
local font = "fangzheng"
local label
if font == 'huakang' then
	label = njname:appendComponent(c"Text", { font = "Assets/font/huakangpipi.xml" })
	label:setText("無敵大忍者陳青青王八蛋")
elseif font == 'fangzheng' then
	label = njname:appendComponent(c"Text", { font = "Assets/font/fangzheng.xft" })
	label:setText("无敌大忍者陈青青王八蛋")
else
	label = njname:appendComponent(c"Text", { font = "Assets/font/speed.xml" })
	label:setText("Nin ja 007")
end

label:setSize(1, 7, 7)
njname:concatTranslate(math3d.vector(0, -1, 0))

local cameraObject = scene:createObject(c"camera")
local camera = cameraObject:appendComponent(c"Camera")
cameraObject:concatTranslate(math3d.vector(0, 8, -25))
