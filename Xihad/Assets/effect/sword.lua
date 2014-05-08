require "math3d"
local Utils = require "ParticleUtils"
local AssetsPath = "../Xihad/Assets/gfx/"

return function(pnode, f, env)
----------------------------------------------
	local scale = env.scale or 1
	
	local src = env:getNode("@source")
	local tar = env:getNode("@target")
	local srcCenter = env:getPosition(src) + env:getAABB(src):center()
	local tarCenter = env:getPosition(tar) + env:getAABB(tar):center()
	local direction = tarCenter - srcCenter

	pnode:setPosition(srcCenter)
	pnode:setParticlesAreGlobal(false)
	
	local width = 0.5 * scale
	local height = 3 * scale
	local img = AssetsPath.."sword.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xffffff66))
	bi:setLifeTime(1.5)
	bi:setWidth(width)
	bi:setHeight(height)

	local boxi = f:initer("Box")
	boxi:setBox(Utils.Box(direction * math3d.vector(0.2, 0.2, 0.2), 0, 5, 5))
	boxi:setNormalDirectionSize(0)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(1200)	
	e:setIniter(f:initer({ bi, boxi }))
	pnode:addEmitter(e, 0, 0.1)
	
	-- renderer
	local r = f:renderer("Direction")
	Utils.addEquilRegion(r, 1 ,2)
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	mat:setBlend("src.alpha", "1-src.alpha", "add")
	mat:setMaterialType("trans_add")
	mat:setLighting(false)		-- Default false
	mat:setZWriteEnable(true)	-- For transparent object, set it to false. Default true
	pnode:setRendererTexture(r, 0, img)	-- 由于支持多重纹理，所以需要指明纹理层

	-- affector
	
	local a = f:affector("Attraction")
	a:setPoint(tarCenter - srcCenter)
	a:setMass(5000)
	pnode:addAffector(a, 0)
	
---------------------------------------------- 	
	env:deferMessage(0.1, "attack begin")
	env:deferMessage(1.5, "attack end")
end
