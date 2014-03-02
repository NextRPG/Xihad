require "math3d"

local function createIniter(f)
	-- local ai = f:initer("AnimatedMesh")
	-- ai:setNormalDirectionSize(2, 3)
	-- ai:setAnimatedMesh("@parent")	-- The parent of pnode should be IAnimatedSceneNode
	
	-- local mi = f:initer("Mesh")
	-- mi:setMesh("@parent")			-- The parent of pnode should be IMeshSceneNode
	-- mi:setMesh("Assets/xxx.obj")	-- The specified string should be a valid mesh file path
	
	
	local bi = f:initer("Basic")
	bi:setColor(0xff104000, 0xff800090)
	bi:setLifeTime(2, 2)
	bi:setWidth(0.4, 0.6)
	bi:setHeight(0.4, 0.6)
	
	local xi = f:initer("Sphere")
	-- xi:setCenter(math3d.vector(0, -3, 0))
	xi:setNormalDirectionSize(-25)
	xi:setRadius(20)
	xi:setOutlineOnly(true)
	
	return f:initer({ bi, xi })
end

return function(pnode, f)
----------------------------------------------
-- + emitter
	local e = f:emitter()
	e:setPPS(1000)	-- Equals to setMinPPS(600), setMaxPPS(1000)
	e:setIniter(createIniter(f))
	pnode:addEmitter(e, 0, 0.5)
	
	-- You must grab emitter if you want to add emitter again!
	-- Because every set/add function will decrease the parameter object's reference count.
	-- addEmitter/setIniter/setRenderer/addAffector
	-- e:grab()
	-- pnode:addEmitter(e, 4) 
	
----------------------------------------------
-- 1 renderer
	-- local r = f:renderer("Point")
	-- r:setPointSize(3)
	-- pnode:setRenderer(r)
	
	local r = f:renderer("Billboard")
	pnode:setRenderer(r)
	-- r:addTexRegion(0,0,0.5,0.5)
	-- r:addTexRegion(0.5,0,1,0.5)
	-- r:addTexRegion(0,0.5,0.5,1)
	-- r:addTexRegion(0.5,0.5,1,1)
	
---------------------------------------------- 
-- * affector
	-- local a = f:affector("FadeOut")
	-- a:setTargetColor(0x22222200)
	-- pnode:addAffector(a, 0.5, 0.7)

	local a = f:affector("FadeOut")
	a:setTargetColor(0xffff0000)
	a:setColorMask("rgb")
	pnode:addAffector(a, 0.5, 0.8)
	
	local a = f:affector("Attraction")
	a:setMass(600)
	a:setRadius(0.5)
	pnode:addAffector(a, 0, 0.5)

	local a = f:affector("Rotation")
	a:setSpeed(math3d.vector(0,0,9))
	pnode:addAffector(a, 0, 0.5)
	
	local a = f:affector("Attraction")
	a:setMass(-50000)
	pnode:addAffector(a, 0.7)
	
	-- local a = f:affector("Force")
	-- a:setGravity(math3d.vector(0, 0, 200))
	-- pnode:addAffector(a, 0.7)
	
	local a = f:affector("Rotation")
	a:setSpeed(math3d.vector(0, 0, 2000))
	pnode:addAffector(a, 0.7)
end
