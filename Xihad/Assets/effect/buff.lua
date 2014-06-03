require "math3d"
local Utils = require "effect.ParticleUtils"

local function addBuff(pnode, f, color, radius, width, height, pps, img, len, speed)
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0))
	bi:setWidth(width)
	bi:setHeight(height)
	bi:setLifeTime(len/speed)
	
	local ci = f:initer("Cylinder")
	ci:setOutlineOnly(true)
	ci:setLength(0)
	ci:setRadius(radius)
	ci:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, speed, 0))
	
-- + emitter
	local e = f:emitter()
	e:setPPS(pps)	-- Equals to setMinPPS(600), setMaxPPS(1000)
	e:setIniter(f:initer({bi, ci, di}))
	pnode:addEmitter(e, 0)

----------------------------------------------
-- 1 renderer
	local r = f:renderer("Quad")
	r:setOrintation(math3d.vector(0, 1, 0), math3d.vector(1, 0, 0))
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath(img))	-- 由于支持多重纹理，所以需要指明纹理层
	
---------------------------------------------- 
-- * affector
	local a = f:affector("FadeOut")
	a:setTargetColor(color)
	a:setColorMask("argb")
	pnode:addAffector(a, 0, 0.4)
	
	Utils.addTransparentAffector(pnode, f, 0.6)
end


return function(pnode, f, env, scale, color)
----------------------------------------------	
	color = color or Color.new(0xffffff)
	scale = scale or 1
 	local tar = env:getNode("@source")
	local extent = env:getAABB(tar):extent()
	
	local hw,_,hh = (extent*math3d.vector(0.5, 0.5, 0.5)):xyz()
	local _,height = (extent*math3d.vector(0, 1, 0)):xyz()
	local r = (hw + hh)/4
	
	local rgbSum = color:getRed() + color:getGreen() + color:getBlue()
	local flowColor = Utils.colorMul(color, 0xaa/rgbSum)
	local shiningColor = Utils.colorAdd(Utils.colorMul(color, 0x77/rgbSum), 0x33)
	
	addBuff(pnode, f, flowColor, r, 1*scale, 10*scale, 5, "flare.bmp", height, 4)
	addBuff(pnode:newChild(), f, shiningColor, r*1.2, 0.7*scale, 0.7*scale, 3, "shining.png", height, 2)
	scale = scale or 1
		
end
