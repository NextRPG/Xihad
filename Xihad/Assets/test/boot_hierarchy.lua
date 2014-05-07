require "math3d"
package.path = package.path..";../Xihad/Assets/test/?.lua"

g_scene:requireSystem(c"Render")

local param = { mesh  = "Assets/model/ninja/ninja.b3d",  clips = "Assets/model/ninja/ninja.clip" }
local function addChildObject(parent, level)
	local left = g_scene:createUniqueObject(c"ninja", parent)
	local anim = left:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle")
	anim:setTransitionTime(0.1)
	left:concatTranslate(math3d.vector(-5, 5, 0))
	left:appendComponent(c"ControlledMove")

	local right = g_scene:createUniqueObject(c"ninja", parent)
	local anim = right:appendComponent(c"AnimatedMesh", param)
	anim:playAnimation(c"idle")
	anim:setTransitionTime(0.1)
	right:concatTranslate(math3d.vector(5, 5, 0))
	right:appendComponent(c"ControlledMove")
	
	if level > 0 then
		addChildObject(left, level-1)
		addChildObject(right, level-1)
	end
end

local function addnilobject(parent, level)
	local left = g_scene:createUniqueObject(c"ninja", parent)
	left:appendComponent(c"Mesh")
	local right = g_scene:createUniqueObject(c"ninja", parent)
	right:appendComponent(c"Mesh")
	
	if level > 1 then
		addnilobject(left, level-1)
		addnilobject(right, level-1)
	end
end

local function addstaticobject(n)
	for i=1,n do
		local st = g_scene:createUniqueObject(c"static")
		st:appendComponent(c"Mesh")
	end
end

local root = g_scene:createObject(c"ninja")
local anim = root:appendComponent(c"AnimatedMesh", param)
anim:playAnimation(c"idle")
anim:setTransitionTime(0.1)
root:concatRotate(math3d.vector(0, 180, 0))

addChildObject(root, 3)
--addnilobject(root, 10)
addstaticobject(2^18)

local playerSelector = {
	onKeyDown = function (self, e)
		local i = tonumber(e.key)
		if not i then 
			print("unrecognize key ", e.key)
			return 1
		end
		
		local name
		if i == 1 then
			name = "ninja"
		else
			name = "ninja".."#"..i
		end
		
		local obj = g_scene:findObject(c(name))
		if obj then
			local ctrl = obj:findComponent(c"ControlledMove")
			if ctrl then
				g_scene:popController()
				ctrl:acquire()
				return 0
			else
				print("no controller") io.flush()
				return -1
			end
		else
			print("no object", name) io.flush()
			return -2
		end
		
		return -99
	end
	}
g_scene:pushController(playerSelector)
playerSelector:drop()

root:appendComponent(c"ControlledMove", moveParam):acquire()
local co = g_scene:createObject(c"co")
co:concatTranslate(math3d.vector(0, 8, -25))
local cameraControl = co:appendComponent(c"Camera")
