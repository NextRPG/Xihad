require "math3d"
local Utils = require "effect.ParticleUtils"
local AssetsPath = "../Xihad/Assets/gfx/"

local function addMagicCircle(pnode, f, lifetime, scale, color)
	local width = 15 * scale
	local height = width
	local img = AssetsPath.."magic_circle_1.png"
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(color)
	bi:setLifeTime(lifetime)
	bi:setWidth(0.2 * scale)
	bi:setHeight(0.2 * scale)
	
	local boxi = Utils.Box(math3d.vector())
	
	-- emitter
	local e = f:emitter()
	e:setPPS(100)	
	e:setIniter(f:initer({ bi, boxi }))
	pnode:addEmitter(e, 0, 0.01)
	
	-- renderer
	local r = f:renderer("Quad")
	r:setOrintation(math3d.vector(0, 0, 1), math3d.vector(1, 0, 0))
	pnode:setRenderer(r)
	
	local mat = r:getMaterial()
	Utils.setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, img)	
	
	-- affector
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	pnode:addAffector(a, 0, 0.1/lifetime)
	
	local a = f:affector("Scale")
	a:setTargetSize(0, 0)
	pnode:addAffector(a, 1 - 0.1/lifetime)
end

return function(pnode, f, env, lifetime, size, color)	
----------------------------------------------
	size = size or 15
	lifetime = lifetime or 3
	color = color or Color.new(0xffffffff)
	
	addMagicCircle(pnode, f, lifetime, size/15, color)
end
