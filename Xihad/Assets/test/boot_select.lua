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
ninja:concatRotate(0, 180, 0)

local cameraObject = scene:createObject(c"camera")
local camera = cameraObject:appendComponent(c"Camera")
cameraObject:concatTranslate(0, 8, -25)

local mouseSelector = {}
function mouseSelector:onMouseEvent(e)
	if e.type == "lPressed" then
		local ray = collision:getRayFromScreenCoord(cursor:getPosition())
		local object, point = collision:detect(ray)
		if object then
			print ("selected object: ", object:getID())
		end
	elseif e.type == "rPressed" then
		local ray = collision:getRayFromScreenCoord(cursor:getPosition())
		local point = collision:detect(ray, "ninja")
		if point then
			print(point:xyz())
		else
			print("No point find")
		end
	end

	return true
end
scene:pushController(mouseSelector)
