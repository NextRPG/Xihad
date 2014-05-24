-- 破冰刃
require "math3d"
local Utils = require "effect.ParticleUtils"
local IniterCreator = Utils.IniterCreator
local iceLife = 2

local function createBox(boxInfo)
	local centerY = boxInfo.centerY or 0
	local dir = boxInfo.dir:copy()
	local len = boxInfo.len
	local w = boxInfo.width or 0
	
	local box = math3d.aabb(math3d.vector(0, centerY, 0))
	for i=0,3 do
		dir:rotate("y", 90 * i)
		dir:normalize()
		box:addPoint(dir * (i%2 == 0 and len/2 or w/2))		
	end
	return box
end

local function setMaterial(mat)
	Utils.setMaterial(mat)
end

local function makeIce(pnode, f, size, delay, pps, boxInfo)
	local width = size
	local height = 7 * width
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0x888888), Color.new(0xffffff))
	bi:setLifeTime(iceLife)
	bi:setWidth(0)
	bi:setHeight(0)

	local xi = f:initer("Box")
	xi:setBox(createBox(boxInfo))
	xi:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(70)
	di:setDirection(math3d.vector(0, 0.000000001, 0))
	
	-- emitter
	local e = f:emitter()
	e:setPPS(pps)	
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, delay, delay+0.01)
	
	-- renderer
	local r = f:renderer("Direction")
	pnode:setRenderer(r)
	
	setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath("ice.png"))	-- 由于支持多重纹理，所以需要指明纹理层
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	pnode:addAffector(a, 0, 0.06/iceLife)
	
	Utils.addTransparentAffector(pnode, f, 0.7)
end

local function makeIceFragment(pnode, f, size, delay, boxInfo)
	local width = 0.8 * size
	local height = 0.8 * size
	
	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(0.6*size -0.1)
	bi:setWidth(width)
	bi:setHeight(height)

	local xi = f:initer("Box")
	xi:setBox(createBox(boxInfo))
	xi:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(50)
	di:setDirection(math3d.vector(0, 11*size + 7, 0))
	
	-- emitter
	local e = f:emitter()
	e:setPPS(800/size)	
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, delay+0.01, delay+0.02)
	
	-- renderer
	local r = f:renderer("Direction")
	Utils.addEquilRegion(r, 4, 4)
	pnode:setRenderer(r)

	setMaterial(r:getMaterial())	
	pnode:setRendererTexture(r, 0, Utils.getImagePath("ice_fragment.jpg"))	-- 由于支持多重纹理，所以需要指明纹理层
	
	local a = f:affector("Force")
	a:setGravity(math3d.vector(0, -60, 0))
	pnode:addAffector(a)
	
	Utils.addTransparentAffector(pnode, f, 0.8)
end

local function makeCoolAir(pnode, f, size, delay, boxInfo)
	local width = 4 * size
	local height = width

	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0))
	bi:setLifeTime(2)
	bi:setWidth(width/2)
	bi:setHeight(height/2)

	local xi = f:initer("Box")
	boxInfo.centerY = height * 0.5
	xi:setBox(createBox(boxInfo))
	xi:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(180)
	di:setDirection(math3d.vector(0, 0.6, 0))
	
	-- emitter
	local e = f:emitter()
	e:setPPS(10/size)	
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, delay, delay + iceLife*0.75)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 2, 2)
	pnode:setRenderer(r)
	
	setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath("explosion.bmp"))	-- 由于支持多重纹理，所以需要指明纹理层
	
	local a = f:affector("Force")
	a:setGravity(math3d.vector(0, -0.1, 0))
	a:setForce(math3d.vector())
	pnode:addAffector(a, 0)
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	pnode:addAffector(a, 0.2)
	
	local a = f:affector("FadeOut")
	a:setTargetColor(Color.new(0x222222))
	a:setColorMask("rgb")
	pnode:addAffector(a, 0, 0.2)
	
	Utils.addTransparentAffector(pnode, f, 0.21)
end

local function addShining(pnode, f, size, delay, boxInfo)
	local width = 2*size
	local height = width

	-- intier
	local bi = f:initer("Basic")
	bi:setLifeTime(1)
	bi:setWidth(0)
	bi:setHeight(0)

	local xi = f:initer("Box")
	boxInfo.centerY = height
	xi:setBox(createBox(boxInfo))
	xi:setNormalDirectionSize(0)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(180)
	di:setDirection(math3d.vector(0, 0.6, 0))
	
	-- emitter
	local e = f:emitter()
	e:setPPS(300)	
	e:setIniter(f:initer({ bi, xi, di }))
	pnode:addEmitter(e, delay, delay + 0.01)
	
	-- renderer
	local r = f:renderer("Direction")
	pnode:setRenderer(r)

	setMaterial(r:getMaterial())	
	pnode:setRendererTexture(r, 0, Utils.getImagePath("shining.png"))	-- 由于支持多重纹理，所以需要指明纹理层
	
	local a = f:affector("Scale")
	a:setTargetSize(width, height)
	pnode:addAffector(a, 0, 0.1)
	
	local a = f:affector("Scale")
	a:setTargetSize(0, 0)
	pnode:addAffector(a, 0.11, 0.2)
end

return function(pnode, f, env, scale)
----------------------------------------------
	pnode:setParticlesAreGlobal(false)
	scale = scale or 1

	local src = env:getNode("@source")
	local tar = env:getNode("@target")
	local srcPos = env:getPosition(src)
	local tarPos = env:getPosition(tar)
	local direction = tarPos - srcPos
	
	local len = direction:length() * 3
	direction:normalize()
	local startPos = srcPos
	local layer = 25
	local timeGap = 0.02
	local minSize = 0.5 * scale
	local maxSize = 2.2 * scale

	local interpolater = function (num)
		return num/layer * len
	end
	
	local sizeIniterpolator = function (num)
		return num/layer * (maxSize - minSize) + minSize
	end
	
	local function createBoxInfo(direction, length, width)
		return { dir = direction, len = length, width = width}
	end
	
	for i=1,layer do
		local length = interpolater(i) - interpolater(i-1)
		local size = sizeIniterpolator(i)
		local node = pnode:newChild()
		local delay = i*timeGap
		node:setPosition(direction * interpolater(i-1) + startPos)
		local pps = length*200/size/size + 150
		makeIce(node, f, size, delay, pps, createBoxInfo(direction, length, 0.1*scale))
		makeIce(node:newChild(), f, size/2, delay, pps, createBoxInfo(direction, length, 0.2*scale))
		makeIceFragment(node:newChild(), f, size, delay, createBoxInfo(direction, length))
		makeCoolAir(node:newChild(), f, size, delay, createBoxInfo(direction, length))
		addShining(node:newChild(), f, size, delay, createBoxInfo(direction, length))
	end
	
---------------------------------------------- 	
	env:deferMessage(layer*timeGap, "attack begin")
	
	-- TODO 过长
	env:deferMessage(layer*timeGap+iceLife*1.3, "attack end")
end
