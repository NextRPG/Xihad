require "math3d"

local function createIniter(f)
	-- local ai = f:initer("AnimatedMesh")
	-- ai:setNormalDirectionSize(2, 3)
	-- ai:setAnimatedMesh("@parent")	-- The parent of pnode should be IAnimatedSceneNode
	
	-- local mi = f:initer("Mesh")
	-- mi:setMesh("@parent")			-- The parent of pnode should be IMeshSceneNode
	-- mi:setMesh("Assets/xxx.obj")	-- The specified string should be a valid mesh file path
	
	
	local bi = f:initer("Basic")
	bi:setColor(0xffffff00, 0xffff0000)
	bi:setLifeTime(2, 3)
	bi:setWidth(2, 4)	-- Useless to point renderer
	bi:setHeight(3, 3)	-- Useless to point renderer
	
	local di = f:initer("Direction")
	di:setDirection(math3d.vector(0,-5,0))
	di:setMaxAngleDegree(120)
	
	local xi = f:initer("Box")
	local box = math3d.aabb(math3d.vector(-1,-1,-1))
	box:addPoint(math3d.vector(1,1,1))
	xi:setBox(box)
	
	return f:initer({ di, bi, xi })
end

return function(pnode, f)
----------------------------------------------
-- + emitter
	local e = f:emitter()
	e:setPPS(2000, 3000)	-- Equals to setMinPPS(600), setMaxPPS(1000)
	e:setIniter(createIniter(f))
	pnode:addEmitter(e, 0, 1)
	
	-- You must grab emitter if you want to add emitter again!
	-- Because every set/add function will decrease the parameter object's reference count.
	-- addEmitter/setIniter/setRenderer/addAffector
	e:grab()
	pnode:addEmitter(e, 4)
	
----------------------------------------------
-- 1 renderer
	local r = f:renderer("Point")
	r:setPointSize(4)
	pnode:setRenderer(r)
	
	
---------------------------------------------- 
-- * affector
	local a = f:affector("FadeOut")
	a:setTargetColor(0x00000000)
	a:setColorMask("agb")
	pnode:addAffector(a, 0.3, 1)
	
	local a = f:affector("Force")
	a:setGravity(math3d.vector(0, 1, 0))
	a:setForce(math3d.vector(5, 0, 0))
	pnode:addAffector(a)
end