require "math3d"
local Utils = require "ParticleUtils"
local IniterCreator = Utils.IniterCreator
local AssetsPath = "../Xihad/Assets/gfx/"

local function makeFire(pnode, f, env, scale, level)
	level = level or 1
	
	local width = 2 * scale
	local height = width
	local radius = 2 * scale / level
	local img = AssetsPath.."fire2.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0, 255, 100, 33), Color.new(102, 255, 120, 33))
	bi:setLifeTime(1.5, 2.0)
	bi:setWidth(width/4)
	bi:setHeight(height/4)

	local xi = f:initer("Box")
	xi:setBox(Utils.Box(math3d.vector(0, height/3, 0), radius, 0, radius))
	xi:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, 2.5 * (scale + 1)/2 , 0))
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100/level)	
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, 0 + 0.5/level, 2 + level/3)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 2 ,2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("1", "0", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	local a = f:affector("Scale")
	a:setTargetSize(width*2, height*2)
	pnode:addAffector(a, 0, 0.4)
	
	local a = f:affector("Scale")
	a:setTargetSize(0, 0)
	pnode:addAffector(a, 0.4, 1.0)
	
	local force = f:affector("Force")
	force:setGravity(math3d.vector(0, 5 * scale, 0))
	pnode:addAffector(force)
	
	Utils.addTransparentAffector(pnode, f, 0.5)
end

local function makeSmoke(pnode, f, env, scale)
	local width = 2 * scale
	local height = width * 1.6
	local radius = 3 * scale
	local img = AssetsPath.."explosion.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xff333333))
	bi:setLifeTime(2)
	bi:setWidth(width/8, width/6)
	bi:setHeight(height/8, height/6)
	
	local xi = IniterCreator.sphere(f, -0.1, radius, false)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, 5 * (scale + 1)/2, 0))
	
	-- emitter
	local e = f:emitter()
	e:setPPS(50)	
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, 1, 3)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 2, 2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	Utils.addTransparentAffector(pnode, f, 0)
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	pnode:addAffector(a, 0, 0.5)
	
	local a = f:affector("Scale")
	a:setTargetSize(0, 0)
	pnode:addAffector(a, 0.5)
	
	local force = f:affector("Force")
	force:setForce(math3d.vector())
	force:setGravity(math3d.vector(0, 2, 0))
	pnode:addAffector(force, 0.2)
end
	

return function(pnode, f, env)
----------------------------------------------
	local scale = env.scale or 1
	
	local tar = env:getNode("@target")

	pnode:setPosition(env:getPosition(tar))
	pnode:setParticlesAreGlobal(false)
	
	local magic = require "magic_circle"
	magic(pnode:newChild(), f, env, 3.5, 20*scale, Color.new(0xffff8833))
	
	
	pnode = pnode:newChild()
	pnode:setPosition(math3d.vector(0, -1, 0))
	
	makeFire(pnode, f, env, scale)
	makeFire(pnode:newChild(), f, env, scale, 1.5)
	makeFire(pnode:newChild(), f, env, scale, 2.5)
	
	local smokeNode = pnode:newChild()
	smokeNode:setPosition(math3d.vector(0, 3*scale, 0))
	makeSmoke(smokeNode, f, env, scale)
	
---------------------------------------------- 	
	env:deferMessage(0.8, "attack begin")
	env:deferMessage(3.5, "attack end")
end
