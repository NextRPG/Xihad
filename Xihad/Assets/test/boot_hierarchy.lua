require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"

scene:requireSystem(c"Render")
local cubeMesh = geometry:createCube(50, 5, 30)
local floor = scene:createObject(c"floor")
local fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
cubeMesh:drop()
fcomp:setColor(255, 0, 0)
floor:concatTranslate(math3d.vector(0, -2.5, 0))

local param = { mesh  = "Assets/model/ninja.b3d",  clips = "Assets/model/ninja.clip" }
local function addChildObject(parent, level)
	local left = scene:createUniqueObject(c"ninja", parent)
	local anim = left:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle 1")
	anim:setTransitionTime(0.1)
	left:concatTranslate(math3d.vector(-5, 5, 0))
	left:concatRotate(math3d.vector(0, 180, 0))

	local right = scene:createUniqueObject(c"ninja", parent)
	local anim = right:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle 1")
	anim:setTransitionTime(0.1)
	right:concatTranslate(math3d.vector(5, 5, 0))
	right:concatRotate(math3d.vector(0, 180, 0))
	
	if level > 0 then
		addChildObject(left, level-1)
		addChildObject(right, level-1)
	end
end

local function addnilobject(parent, level)
	local left = scene:createUniqueObject(c"ninja", parent)
	local right = scene:createUniqueObject(c"ninja", parent)
	if level > 0 then
		addnilobject(left, level-1)
		addnilobject(right, level-1)
	end
end

local root = scene:createObject(c"ninja")
local anim = root:appendComponent(c"AnimatedMesh", param)
anim:playAnimation(c"idle 1")
anim:setTransitionTime(0.1)
root:concatRotate(math3d.vector(0, 180, 0))

addChildObject(root, 3)
addnilobject(root, 18)
root:appendComponent(c"ControlledMove", moveParam):acquire()

local co = scene:createObject(c"co")
co:concatTranslate(math3d.vector(0, 8, -25))
local cameraControl = co:appendComponent(c"Camera")
