-- 月光斩

require "math3d"
local Utils = require "ParticleUtils"
local IniterCreator = Utils.IniterCreator
local AssetsPath = "../Xihad/Assets/gfx/"
local bladeLife = 0.5	
local defaultBlendType = "add"

local function addWindBlade(pnode, f, scale, dir, getRender)
	local width = 5 * scale
	local height = width*2
	local img = AssetsPath.."wind.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0xffffffff))
	bi:setLifeTime(bladeLife)
	bi:setWidth(width)
	bi:setHeight(height)
	
	local ci = f:initer("Box")
	ci:setBox(Utils.Box(math3d.vector(), 0, 0.5, 0))
	ci:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(dir*2)
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100)	
	e:setIniter(f:initer({ bi, ci, di }))
	pnode:addEmitter(e, 0, 0.01)
	
	-- renderer
	local r = getRender()
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial(), nil, "1", defaultBlendType)
	pnode:setRendererTexture(r, 0, img)	
	

	local a = f:affector("Rotation")
	a:setPivot(math3d.vector())
	a:setSpeed(dir)
	-- pnode:addAffector(a)
	
	Utils.addTransparentAffector(pnode, f, 0.8)
end

local function addBlood(pnode, f, scale)
	local width = 0.3 * scale
	local height =  width
	local img = AssetsPath.."blood.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(0.4)
	bi:setWidth(width)
	bi:setHeight(height)
	
	local xi = IniterCreator.sphere(f, 4, 0.1 * scale, true) 
	
	-- emitter
	local e = f:emitter()
	e:setPPS(200)
	e:setIniter(f:initer({ bi, xi }))
	pnode:addEmitter(e, bladeLife - 0.1, bladeLife + 0.1)
	
	-- renderer
	local r = f:renderer("Direction")
	Utils.addEquilRegion(r, 1, 2)
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial(), nil, nil, nil, "trans_alphach", false)
	pnode:setRendererTexture(r, 0, img)

end

return function(pnode, f, env, scale, blendType)	
----------------------------------------------
	scale = scale or 1
	scale = scale*1.5

	defaultBlendType = blendType or defaultBlendType
	local src = env:getNode("@source")
	local tar = env:getNode("@target")
	
	local dir = env:getPosition(tar) - env:getPosition(src)
	color = color or defaultColor
	
	pnode:setPosition(env:getPosition(src) + env:getAABB(src):extent()*math3d.vector(0, 0.5, 0))
	pnode:setParticlesAreGlobal(false)
	addWindBlade(pnode:newChild(), f, scale, dir, 
		function ()
			local r = f:renderer("Quad")
			r:setOrintation(math3d.vector(0, 0, 1), dir)
			return r	
		end)
	
	addWindBlade(pnode:newChild(), f, scale, dir, 
		function ()
			local r = f:renderer("Quad")
			r:setOrintation(math3d.vector(1, 0, 0), dir)
			return r	
		end)

	local blood = pnode:newChild()
	blood:setPosition(dir)
	addBlood(blood, f, scale)
	
	
-------------------------------------------
	env:deferMessage(bladeLife, "attack begin")
	env:deferMessage(bladeLife+0.1, "attack end")
end
