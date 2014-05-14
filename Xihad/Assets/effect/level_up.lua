require "math3d"
local Utils = require "ParticleUtils"
local color = Color.new(Color.gold)
local life = 1

local function addLightBeam(pnode, f, scale, radius, height)
	local width = 1*scale
	
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0), color)
	bi:setWidth(0)
	bi:setHeight(0)
	bi:setLifeTime(life)
	
	local ci = f:initer("Cylinder")
	ci:setOutlineOnly(true)
	ci:setLength(0)
	ci:setRadius(radius)
	ci:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, 0.000000001, 0))
	
	local e = f:emitter()
	e:setPPS(200)	
	e:setIniter(f:initer({bi, ci, di}))
	pnode:addEmitter(e, 0, 0.5)

	local r = f:renderer("Direction")
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath("buff.png"))
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height*2)
	pnode:addAffector(a, 0, 0.4)
	
	Utils.addTransparentAffector(pnode, f, 0.5)
end

local function addSpark(pnode, f, area, radius, height, life, color)
	local bi = f:initer("Basic")
	bi:setColor(color)
	bi:setWidth(area.w)
	bi:setHeight(area.h)
	bi:setLifeTime(life)
	
	local ci = f:initer("Cylinder")
	ci:setOutlineOnly(false)
	ci:setLength(0)
	ci:setRadius(radius)
	ci:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, height/life, 0))
	
	local e = f:emitter()
	e:setPPS(5)	
	e:setIniter(f:initer({bi, ci, di}))
	pnode:addEmitter(e, 0, 1.5)

	local r = f:renderer("Billboard")
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath("shining.png"))
	
	Utils.addTransparentAffector(pnode, f, 0.5)
end

return function(pnode, f, env, scale)
----------------------------------------------	
	scale = scale or 1
 	local src = env:getNode("@source")
 	pnode:setPosition(env:getPosition(src))
 	
	local extent = env:getAABB(src):extent()
	local hw,_,hh = (extent*math3d.vector(0.5, 0, 0.5)):xyz()
	local _,height = (extent*math3d.vector(0, 1, 0)):xyz()
	local r = (hw + hh)/2
	
	addLightBeam(pnode:newChild(), f, scale, r, height*1.2)
	addLightBeam(pnode:newChild(), f, scale, r*0.75, height*1.35)
	addLightBeam(pnode:newChild(), f, scale, r*0.5, height*1.5)
	
	addSpark(pnode:newChild(), f, { w =0.5*scale, h = 0.5*scale}, r, height*1.5, 2, Color.new(0x33dddd))
	addSpark(pnode:newChild(), f, { w =0.5*scale, h = 0.5*scale}, r, height*1.5, 2, Color.new(0xdddd33))
	addSpark(pnode:newChild(), f, { w =0.5*scale, h = 0.5*scale}, r, height*1.5, 2, Color.new(0xdd33dd))
end
