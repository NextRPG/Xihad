require "math3d"

local cubeMesh = geometry:createCube(50, 5, 30)
local floor = scene:createObject(c"floor")
local fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:setColor(255, 0, 0)
floor:concatTranslate(0, -2.5, 0)

local param = { 
	mesh  = "Assets/model/ninja.b3d", 
	clips = "Assets/model/ninja.clip" 
}

for i = 10, 1, -1 do
	ninja = scene:createObject(c("ninja"..i))
	anim = ninja:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle 1")
	anim:setTransitionTime(0.1)
	anim:addShadow()
	if i%2==0 then 
		ninja:concatTranslate(-i/2*3, 0, i/2*3)
	else
		ninja:concatTranslate(i/2*3, 0, i/2*3)
	end

	ninja:concatRotate(0, 180, 0)
end

ninja:resetTranslate(1, 0, 1)
ninja:appendComponent(c"ControlledMove", moveParam):acquire()

local co = scene:createObject(c"co", ninja)
co:concatTranslate(0, 8, -25)
local cameraControl = co:appendComponent(c"Camera")

-- look at parent 
co:appendUpdater {
	onUpdate = function ()
		cameraControl:setTarget(math3d.vector(ninja:getTranslate()))
	end
}
