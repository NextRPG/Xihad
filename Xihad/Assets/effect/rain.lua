require "math3d"
local color = Color.new

local function createIniter(f)
	local basic = f:initer("Basic")
	basic:setColor(color(0x80dddddd), color(0x80ffffff))
	basic:setLifeTime(1, 1.2)
	basic:setWidth(0.01, 0.02)
	basic:setHeight(0.5, 0.7)
	
	local bi = f:initer("Box")
	local height = 10
	local range = 5
	local box = math3d.aabb(math3d.vector(0, height, 0))
	box:addPoint(math3d.vector(-range, height, 2))
	box:addPoint(math3d.vector(range, height, 5))
	bi:setBox(box)
	bi:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, -10, 0))
	
	return f:initer({ basic, bi, di })
end

return function(pnode, f, env)
----------------------------------------------
-- + emitter
	local e = f:emitter()
	e:setPPS(500)	-- Equals to setMinPPS(600), setMaxPPS(1000)
	e:setIniter(createIniter(f))
	pnode:addEmitter(e)
	
----------------------------------------------
-- 1 renderer
	local r = f:renderer("Quad")
	r:setOrintation(math3d.vector(0,1,0), math3d.vector(1,0,0))
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1 - src.alpha", "add")
	mat:setMaterialType("trans_alphach")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(false)	-- For transparent object, set it to false. Default true
	
---------------------------------------------- 
-- * affector
	local a = f:affector("FadeOut")
	a:setTargetColor(color(Color.transparent))
	a:setColorMask("a")
	pnode:addAffector(a, 0.5)
	
	local force = f:affector("Force")
	force:setGravity(math3d.vector(0,-3, 0))
	force:setForce(math3d.vector(0.003, 0, 0))
	pnode:addAffector(force)
end