require "math3d"
local Utils = require "ParticleUtils"
local IniterCreator = Utils.IniterCreator
local AssetsPath = "../Xihad/Assets/gfx/"

local MinColor = Color.new(0xffc78ef8)
local MaxColor = Color.new(0xff5d4abc)
local CollisionTime = 0.2
local LifeTime = 0.8

local function addExplosion(pnode, f, env, scale)
	local width = 5 * scale
	local height = width
	local radius = 1 * scale
	local img = AssetsPath.."flare.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(LifeTime)
	bi:setColor(Color.new(0xffc78ef8), Color.new(0xff5d4abc))
	bi:setWidth(width, 1.4*width)
	bi:setHeight(height, 1.4*height)
	
	local boxi = f:initer("Box")
	boxi:setBox(Utils.Box(math3d.vector(), radius, radius/2, radius))
	boxi:setNormalDirectionSize(0.0000001)
		
	-- emitter
	local e = f:emitter()
	e:setPPS(500)
	e:setIniter(f:initer({ bi, boxi }))
	pnode:addEmitter(e, CollisionTime, CollisionTime + 0.1)
	
	-- renderer
	local r = f:renderer("Direction")
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1", "add")
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(false)	
	pnode:setRendererTexture(r, 0, img)

	Utils.addTransparentAffector(pnode, f)
end

local function addVerticleLightning(pnode, f, env, scale)
	local width = 5 * scale
	local height = 25 * scale
	local img = AssetsPath.."lightning.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(MinColor, MaxColor)
	bi:setLifeTime(LifeTime)
	bi:setWidth(width)
	bi:setHeight(height)

	local boxi = f:initer("Box")
	boxi:setBox(Utils.Box(math3d.vector(0, height/2, 0), 0, 0, 0))
	boxi:setNormalDirectionSize(0)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100)	
	e:setIniter(f:initer({ bi, boxi}))
	pnode:addEmitter(e, CollisionTime, CollisionTime + 0.1)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 2, 4)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	Utils.addTransparentAffector(pnode, f, (CollisionTime+0.1)/LifeTime)	
end

local function addSpark(pnode, f, env, scale)
	local width = 3 * scale
	local height = 20 * scale
	local radius = 4 * scale
	local img = AssetsPath.."lightning.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(LifeTime)
	bi:setColor(MinColor, MaxColor)
	bi:setWidth(width)
	bi:setHeight(height)
	
	local xi = IniterCreator.sphere(f, 0.1, radius, true)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100)
	e:setIniter(f:initer({ bi, xi }))
	pnode:addEmitter(e, CollisionTime, CollisionTime + 0.1)
	
	-- renderer
	local r = f:renderer("Direction")
	Utils.addEquilRegion(r, 2, 4)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setZWriteEnable(false)	
	pnode:setRendererTexture(r, 0, img)

	Utils.addTransparentAffector(pnode, f, 0)
end


local function addHorizontalLightning(pnode, f, env, scale, direction)
	local width = 6 * scale
	local height = 20 * scale
	local radius = 3 * scale
	local img = AssetsPath.."lightning.png"
	local LifeTime = LifeTime * 1.2
	
	pnode:setPosition(direction * height/2)

	-- intier
	local bi = f:initer("Basic")
	bi:setColor(MinColor, MaxColor)
	bi:setLifeTime(LifeTime)
	bi:setWidth(0)
	bi:setHeight(0)

	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(direction * 0.00000001)
		
	-- emitter
	local e = f:emitter()
	e:setPPS(100)	
	e:setIniter(f:initer({ bi, di }))
	pnode:addEmitter(e, 0, 0.1)
	
	-- renderer
	local r = f:renderer("Direction")
	Utils.addEquilRegion(r, 2, 4)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("0", "1", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	pnode:addAffector(a, 0, CollisionTime / LifeTime)
	
	-- affector
	Utils.addTransparentAffector(pnode, f, CollisionTime / LifeTime)	
end

return function(pnode, f, env)
----------------------------------------------
	local scale = env.scale or 1
	
	local tar = env:getNode("@target")
	local tarAABB = env:getAABB(tar)
	pnode:setPosition(env:getPosition(tar))
	pnode:setParticlesAreGlobal(false)
	
	addVerticleLightning(pnode, f, env, scale)
	
	local v = math3d.vector(1, 0.1, 0)
	local count = 5
	for i = 1, count  do
		addHorizontalLightning(pnode:newChild(), f, env, scale, v)
		v:rotate("y", 360/count)
	end
	
	addExplosion(pnode:newChild(), f, env, scale)
	addSpark(pnode:newChild(), f, env, scale)
	
	local magic = require "magic_circle"
	magic(pnode:newChild(), f, env, 1.1, 26*scale, MinColor)
---------------------------------------------- 	
	env:deferMessage(CollisionTime, "attack begin")
	env:deferMessage(1.05, "attack end")
end
