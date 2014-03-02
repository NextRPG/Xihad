require "math3d"

local function createIniter(f)
	local bi = f:initer("Basic")
	-- bi:setColor(0xff1040f0, 0xff808090)
	bi:setColor(0x00000000, 0xff00ffff)
	bi:setLifeTime(2, 2)
	bi:setWidth(0.4, 0.6)
	bi:setHeight(0.4, 0.6)
	
	local xi = f:initer("Sphere")
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

----------------------------------------------
-- 1 renderer
	local r = f:renderer("Billboard")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src_alpha", "1 - src_alpha", "add")
	mat:setMaterialType("trans_alphach")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(false)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, "../Xihad/Assets/gfx/fire2.png")	-- 由于支持多重纹理，所以需要指明纹理层
	
---------------------------------------------- 
-- * affector
	local a = f:affector("FadeOut")
	a:setTargetColor(0x00000000)
	a:setColorMask("rgb")
	pnode:addAffector(a, 0, 0.45)
	
	local a = f:affector("FadeOut")
	a:setTargetColor(0xffff0000)
	a:setColorMask("rgb")
	pnode:addAffector(a, 0.5, 0.8)
	
	local a = f:affector("Attraction")
	a:setMass(1000)
	a:setRadius(0.3)
	pnode:addAffector(a, 0, 0.5)

	local a = f:affector("Rotation")
	a:setSpeed(math3d.vector(0,0,9))
	pnode:addAffector(a, 0, 0.5)
	
	local a = f:affector("Attraction")
	a:setMass(-50000)
	pnode:addAffector(a, 0.7)
	
	local a = f:affector("Rotation")
	a:setSpeed(math3d.vector(0, 0, 2000))
	pnode:addAffector(a, 0.7)
end
