require "math3d"

local cubeMesh = geometry:createCube(15, 5, 30)
local floor = scene:createObject(c"floor")
local fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:setColor(255, 0, 0)
floor:concatTranslate(0, -2.5, 0)

floor = scene:createObject(c"floor2")
fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:setColor(0, 255, 0)
floor:concatTranslate(15, -2.5, 0)

floor = scene:createObject(c"floor3")
fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:setColor(0, 0, 255)
floor:concatTranslate(-15, -2.5, 0)

local param = { 
	-- mesh = "Assets/model/man2_x/2.b3d"
	mesh  = "Assets/model/ninja.b3d", 
	clips = "Assets/model/ninja.clip" 
}

local ninja = nil
local anim = nil

for i = 10, 1, -1 do
	ninja = scene:createObject(c("ninja"..i))
	anim = ninja:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle 1")
	anim:setTransitionTime(0.1)
	-- anim:setAnimationSpeed(0)
	-- anim:addShadow(nil, "zfail")
	if i%2==0 then 
		ninja:concatTranslate(-i/2*3, 0, i/2*3)
	else
		ninja:concatTranslate(i/2*3, 0, i/2*3)
	end

	ninja:concatRotate(0, 180, 0)
end

-- anim:addDebugState("skeleton", "aabb")
-- ninja:concatUniScale(0.3);
ninja:resetTranslate(1, 0, 1)

local moveParam = { speed = 20 }
local move = ninja:appendComponent(c"ControlledMove", moveParam)
move:acquire()

local camera = scene:createObject(c"camera")
camera:appendComponent(c"Camera")
-- euler = math3d.vector(0, -20, 0):horizontalAngle()
-- camera:concatRotate(euler:xyz())
camera:concatTranslate(0, 8, -25)

local sun = scene:createObject(c"sun")
lightComp = sun:appendComponent(c"Light")
lightComp:castShadow(true)
lightComp:setType "point"
sun:concatRotate(90, 0, 0)
sun:concatTranslate(20, 30, -5)
