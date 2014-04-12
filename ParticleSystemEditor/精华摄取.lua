require "math3d"

local function createIniter(f, env)
	local bi = f:initer("Basic")
	-- bi:setColor(0xff1040f0, 0xff808090)
	bi:setColor(Color.new(0x00008000), Color.new(0xff00ff00))
	bi:setLifeTime(0.3)
	
	local ai = f:initer("AnimatedMesh")
	ai:setAnimatedNode(env:getNode("@target"))
	ai:setOutlineOnly(false) -- default: true
	return f:initer({bi, ai})
end

return function(pnode, f, env)
----------------------------------------------
-- locate pnode
-- AnimatedMeshIniter will auto transform point to AnimatedNode
	
	-- local t = env:getNode("@target")
	-- local tpos = env:getPosition(t)
	-- pnode:setPosition(tpos)-- + env:getAABB(t):center())

----------------------------------------------
-- + emitter
	local e = f:emitter()
	e:setPPS(10000)	-- Equals to setMinPPS(600), setMaxPPS(1000)
	e:setIniter(createIniter(f, env))
	pnode:addEmitter(e, 0, 0.8)

----------------------------------------------
-- 1 renderer
	local r = f:renderer("Point")
	r:setPointSize(5)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(false)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, "../Xihad/Assets/gfx/fire2.png")	-- 由于支持多重纹理，所以需要指明纹理层
	
---------------------------------------------- 
-- * affector
	local a = f:affector("FadeOut")
	a:setTargetColor(Color.new(0x20202020))
	a:setColorMask("rgb")
	pnode:addAffector(a)
	
	local a = f:affector("Attraction")
	a:setMass(500000)
	a:setRadius(3)
	local src = env:getNode("@source")
	local tar = env:getNode("@target")
	a:setPoint(env:getPosition(src)*1.2 + env:getAABB(src):center())
	pnode:addAffector(a)--, 0, 0.5)
end
