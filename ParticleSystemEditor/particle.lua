require "math3d"
local Utils = require "ParticleUtils"
local AssetsPath = "../Xihad/Assets/gfx/"

local function addWaterSpout(pnode, f, scale)
	local width = 2 * scale
	local height = width
	local lifetime = 5
	local img = AssetsPath.."water.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xff8888ff), Color.new(0xffffffff))
	bi:setLifeTime(lifetime)
	bi:setWidth(0.2 * scale, 2 * scale)
	bi:setHeight(0.2 * scale, 2 * scale)
	
	local ci = f:initer("Cylinder")
	ci:setOutlineOnly(true)
	ci:setCenter(math3d.vector())
	ci:setRadius(5)
	ci:setNormal(math3d.vector(0, 0.0001, 0))
	ci:setLength(0.1)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(10000)	
	e:setIniter(f:initer({ bi, ci }))
	pnode:addEmitter(e, 0, 3)
	
	-- renderer
	local r = f:renderer("Direction")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "0", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	
	
	-- affector
	local a = f:affector("Attraction")
	a:setMass(1)
	-- pnode:addAffector(a)
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	-- pnode:addAffector(a, 0, 0.1/lifetime)
	
	local a = f:affector("Rotation")
	a:setPivot(math3d.vector())
	a:setSpeed(math3d.vector(100, 0, 0))
	-- pnode:addAffector(a)
	
	local a = f:affector("Force")
	a:setGravity(math3d.vector(0, 10, 0))
	-- pnode:addAffector(a)
end

return function(pnode, f, env)	
----------------------------------------------
	local scale = env.scale or 1
	
	local tar = env:getNode("@target")
	pnode:setPosition(env:getPosition(tar))
	pnode:setParticlesAreGlobal(false)

	addWaterSpout(pnode, f, scale)
end
