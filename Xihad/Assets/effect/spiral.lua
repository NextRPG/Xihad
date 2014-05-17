require "math3d"
local Utils = require "effect.ParticleUtils"
local AssetsPath = "../Xihad/Assets/gfx/"

local function addExplosion(pnode, f, env)
	local width = 0.2 * scale
	local height = 15 * scale
	local radius = 3 * scale
	local img = AssetsPath.."spark2.bmp"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xffc78ef8), Color.new(0xff5d4abc))
	bi:setLifeTime(0.5)
	bi:setWidth(width)
	bi:setHeight(height, height*1.5)

	local ci = f:initer("Sphere")
	-- ci:setHeight(1)
	ci:setRadius(radius)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100)	
	e:setIniter(f:initer({ bi, ci}))
	pnode:addEmitter(e, 0, 1)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 4, 2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	Utils.addTransparentAffector(pnode, f, 0.5)	
end

local function addLightning(pnode, f, env, scale)
	local width = 7.5 * scale
	local height = 15 * scale
	local radius = 0.2 * scale
	local img = AssetsPath.."lightning.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xffc78ef8), Color.new(0xff5d4abc))
	bi:setLifeTime(0.5)
	bi:setWidth(width, width*1.5)
	bi:setHeight(height, height*1.5)

	local boxi = f:initer("Box")
	boxi:setBox(Utils.Box(math3d.vector(0, height/3, 0), 1, 0, 1))
	boxi:setNormalDirectionSize(0)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(25)	
	e:setIniter(f:initer({ bi, boxi}))
	pnode:addEmitter(e, 0, 1)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 4, 2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	
	Utils.addTransparentAffector(pnode, f, 0.5)	
end

return function(pnode, f, env)
----------------------------------------------
	local scale = env.scale or 1
	
	local tar = env:getNode("@target")
	local tarAABB = env:getAABB(tar)
	pnode:setPosition(env:getPosition(tar))
	pnode:setParticlesAreGlobal(false)
	
	addLightning(pnode, f, env, scale)
	addExplosion(pnode:newChild(), f, env, scale)
---------------------------------------------- 	
	env:deferMessage(0.1, "attack begin")
	env:deferMessage(1.5, "attack end")
end
