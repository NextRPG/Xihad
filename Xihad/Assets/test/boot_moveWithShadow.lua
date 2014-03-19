require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"
g_scene:requireSystem(c'Render')

local cubeMesh = g_geometry:createCube(15, 5, 30)
local floor = g_scene:createObject(c"floor")
local fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:getMaterial(0):setColorMaterial('none')
fcomp:getMaterial(0):setDiffuseColor(Color.new(Color.red))
floor:concatTranslate(math3d.vector(0, -2.5, 0))

floor = g_scene:createObject(c"floor2")
fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:getMaterial(0):setColorMaterial('none')
fcomp:getMaterial(0):setDiffuseColor(Color.new(Color.green))
floor:concatTranslate(math3d.vector(15, -2.5, 0))

floor = g_scene:createObject(c"floor3")
fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:getMaterial(0):setColorMaterial('none')
fcomp:getMaterial(0):setDiffuseColor(Color.new(Color.blue))
floor:concatTranslate(math3d.vector(-15, -2.5, 0))

local param = { 
	-- mesh = "Assets/model/man2_x/2.b3d"
	mesh  = "Assets/model/ninja.b3d", 
	clips = "Assets/model/ninja.clip" 
}

local ninja = nil
local anim = nil

for i = 10, 1, -1 do
	ninja = g_scene:createObject(c("ninja"..i))
	anim = ninja:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle 1")
	anim:setTransitionTime(0.1)
	-- anim:setAnimationSpeed(0)
	-- anim:addShadow(nil, "zfail")
	if i%2==0 then 
		ninja:concatTranslate(math3d.vector(-i/2*3, 0, i/2*3))
	else
		ninja:concatTranslate(math3d.vector(i/2*3, 0, i/2*3))
	end

	ninja:concatRotate(math3d.vector(0, 180, 0))
end

-- anim:addDebugState("skeleton", "aabb")
-- ninja:concatUniScale(0.3);
ninja:resetTranslate(math3d.vector(1, 0, 1))

local moveParam = { speed = 20 }
local move = ninja:appendComponent(c"ControlledMove", moveParam)
move:acquire()

local camera = g_scene:createObject(c"camera")
local ccam = camera:appendComponent(c"Camera")
-- euler = math3d.vector(0, -20, 0):horizontalAngle()
-- camera:concatRotate(euler:xyz())
camera:concatTranslate(math3d.vector(0, 30, -25))

ccam:setTarget(math3d.vector(0, 0, -15))
ccam:setUpVector(math3d.vector(0, 1, 0))
camera:appendUpdater({ onUpdate = function()
	local dx = 0.1
	camera:resetTranslate(camera:getTranslate() + math3d.vector(dx, 0,0))
	ccam:setTarget(ccam:getTarget() + math3d.vector(dx,0,0))
end })


local sun = g_scene:createObject(c"sun")
lightComp = sun:appendComponent(c"Light")
lightComp:castShadow(true)
lightComp:setType "point"
sun:concatRotate(math3d.vector(90, 0, 0))
sun:concatTranslate(math3d.vector(20, 30, -5))

-- local guisystem = CEGUI.System:getSingleton()
-- guisystem:executeScriptFile("MySample.lua")