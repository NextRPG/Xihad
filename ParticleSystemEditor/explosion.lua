require "math3d"
local Utils = require "ParticleUtils"
local IniterCreator = Utils.IniterCreator
local AssetsPath = "../Xihad/Assets/gfx/"

local function addFlame(pnode, f, scale, outLineOnly)
	local width = 4 * scale
	local height = width
	local radius = 2 * scale
	local img = AssetsPath.."explosion.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xff333333), Color.new(0xffff5500))
	bi:setLifeTime(0.8, 1.0)
	bi:setWidth(width, width*1.5)
	bi:setHeight(height, height*1.5)
	
	local xi = IniterCreator.sphere(f, 0.1, radius, outLineOnly or false)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(200)	
	e:setIniter(f:initer({ bi, xi }))
	pnode:addEmitter(e, 0.02, 0.2)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 2, 2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("1", "0", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	Utils.addTransparentAffector(pnode, f, 0.2)
	
	local a = f:affector("Scale")
	a:setTargetSize(width* 2, height*2)
	pnode:addAffector(a, 0, 1.0)
end

local function addSmoke(pnode, f, scale)
	local width = 4 * scale
	local height = width
	local radius = 3 * scale
	local img = AssetsPath.."explosion.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xff333333))
	bi:setLifeTime(1.5)
	bi:setWidth(width, width*1.5)
	bi:setHeight(height, height*1.5)
	
	local xi = IniterCreator.sphere(f, 0.1, radius, false)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100)	
	e:setIniter(f:initer({ bi, xi }))
	pnode:addEmitter(e, 0.3, 0.4)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 2, 2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("1-dst.alpha", "dst.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	Utils.addTransparentAffector(pnode, f, 0.6)
	
	local a = f:affector("Scale")
	a:setTargetSize(width*2, height*2)
	pnode:addAffector(a, 0, 1.0)
	
	local force = f:affector("Force")
	force:setForce(math3d.vector())
	force:setGravity(math3d.vector(0, 2, 0))
	pnode:addAffector(force, 0.2)
end

local function addSpark(pnode, f, scale)
	local width = 0.2 * scale
	local height = width
	local radius = 2.5 * scale
	local img = AssetsPath.."point.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(1.4, 1.5)
	bi:setColor(Color.new(0, 255, 255, 0.7*255), Color.new(255, 255, 255, 0.3*255))
	bi:setWidth(width)
	bi:setHeight(height)
	
	local xi = IniterCreator.sphere(f, 4, radius, false)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(1000)
	e:setIniter(f:initer({ bi, xi }))
	pnode:addEmitter(e, 0, 0.1)
	
	-- renderer
	local r = f:renderer("Billboard")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(false)	
	pnode:setRendererTexture(r, 0, img)

	-- affector
	local force = f:affector("Force")
	force:setForce(math3d.vector())
	force:setGravity(math3d.vector(0, -2, 0))
	pnode:addAffector(force)
	
	Utils.addTransparentAffector(pnode, f, 0.6)
end

local function addSpark2(pnode, f, scale)
	local width = 0.15 * scale
	local height = 2 * scale
	local radius = 3 * scale
	local img = AssetsPath.."spark2.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(0.3)
	bi:setColor(Color.new(0, 255, 255, 0.7*255), Color.new(255, 255, 255, 0.3*255))
	bi:setWidth(width)
	bi:setHeight(height)
	
	local xi = IniterCreator.sphere(f, 20, radius, true) 
	
	-- emitter
	local e = f:emitter()
	e:setPPS(120)
	e:setIniter(f:initer({ bi, xi }))
	pnode:addEmitter(e, 0, 0.2)
	
	-- renderer
	local r = f:renderer("Direction")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(false)	
	pnode:setRendererTexture(r, 0, img)

	-- affector
	local force = f:affector("Force")
	force:setForce(math3d.vector())
	force:setGravity(math3d.vector(0, -1, 0))
	pnode:addAffector(force)
	
	Utils.addTransparentAffector(pnode, f, 0.6)
	
	local a = f:affector("Attraction")
	a:setMass(-50)
	pnode:addAffector(a, 0, 0.1)
end

local function addFlash(pnode, f, scale)
	local width = 5 * scale
	local height = width
	local radius = 1 * scale
	local img = AssetsPath.."flash.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(0.5, 0.5)
	bi:setWidth(width, 1.4*width)
	bi:setHeight(height, 1.4*height)
	
	
	local xi = IniterCreator.sphere(f, 0, radius, false)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(40)
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, 0, 0.2)
	
	-- renderer
	local r = f:renderer("Billboard")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1", "add")
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(false)	
	pnode:setRendererTexture(r, 0, img)

	Utils.addTransparentAffector(pnode, f, 0, 0.8)
end

local function addWave(pnode, f, scale)
	local width = 6 * scale
	local height = 1 * scale
	local img = AssetsPath.."wave.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0x44444444))
	bi:setLifeTime(0.5)
	bi:setWidth(width)
	bi:setHeight(height)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(20)
	e:setIniter(f:initer({ bi }))
	pnode:addEmitter(e, 0, 0.1)
	
	-- renderer
	local r = f:renderer("Billboard")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(true)	
	pnode:setRendererTexture(r, 0, img)

	local a = f:affector("Scale")
	a:setTargetSize(width*2.5, height*2.5)
	pnode:addAffector(a, 0, 1)
	
	Utils.addTransparentAffector(pnode, f)
end

return function(pnode, f, env)
----------------------------------------------
	-- locate pnode
	local tar = env:getNode("@target")
	pnode:setPosition(env:getPosition(tar) + env:getAABB(tar):center())
	pnode:setParticlesAreGlobal(false)
	
	local scale = env.scale or 1
	
	addFlash(pnode, f, scale)
	addSmoke(pnode:newChild(), f, scale)
	addSpark(pnode:newChild(), f, scale)
	addSpark2(pnode:newChild(), f, scale)
	addWave(pnode:newChild(), f, scale)
	addFlame(pnode:newChild(), f, scale)
	addFlame(pnode:newChild(), f, scale, true)
---------------------------------------------- 	
	
	env:deferMessage(0.1, "attack begin")
	env:deferMessage(1.5, "attack end")
end
