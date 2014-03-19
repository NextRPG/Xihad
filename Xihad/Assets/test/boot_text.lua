require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"

--- Format loading speed statistics
-- g_scene:requireSystem(c"Render")
-- local fmts = { 'png', 'jpg', 'bmp', 'tga' }
-- for _,fmt in ipairs(fmts) do
-- 	for i=1,10 do
-- 		local tex = g_textureManager:getTexture('Assets/font/fangzheng0.'..fmt)
-- 		g_textureManager:removeTexture(tex)
-- 	end
-- end

local param = { mesh  = "Assets/model/ninja.b3d", clips = "Assets/model/ninja.clip" }
local ninja = g_scene:createObject(c"ninja")
ninja:resetTranslation(math3d.vector(0, 0, -10))
anim = ninja:appendComponent(c"AnimatedMesh", param)
anim:playAnimation(c"idle 1")
anim:setTransitionTime(0.1)
ninja:concatRotate(math3d.vector(0, 180, 0))

local ctrl = ninja:appendComponent(c'ControlledMove')
ctrl:acquire()	-- control ninja

local njname = g_scene:createObject(c"ninjaName", ninja)
local font = "fangzheng"
local label
if font == 'huakang' then
	label = njname:appendComponent(c"Text", { font = "Assets/font/huakangpipi.xft" })
	label:setText("無敵忍者")
elseif font == 'fangzheng' then
	label = njname:appendComponent(c"Text", { font = "Assets/font/fangzheng.xft" })
	label:setText("无敌忍者")
else
	label = njname:appendComponent(c"Text", { font = "Assets/font/speed.xft" })
	label:setText("Nin ja 007")
end

-- Everytime after you invoke setText, the label's width will be adjusted.
label:adjustToHeight(1.5)
label:setColor(200, 200, 100)
njname:concatTranslate(math3d.vector(0, -1, 0))

local cameraObject = g_scene:createObject(c"camera")
local camera = cameraObject:appendComponent(c"Camera")
cameraObject:concatTranslate(math3d.vector(0, 8, -25))
