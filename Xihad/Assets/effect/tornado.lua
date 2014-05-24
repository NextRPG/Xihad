-- 龙卷风

require "math3d"
local Utils = require "effect.ParticleUtils"

local function addWind(pnode, f, delay, pos, width, len)
	pnode:setPosition(pos)
	
	local height = width
	
	-- intier
	local bi = f:initer("Basic")
	bi:setColor(Color.new(0x888888))
	bi:setLifeTime(0.2)
	bi:setWidth(width, width*2)
	bi:setHeight(height, height*2)
	
	local si = f:initer("Cylinder")
	si:setLength(len)
	si:setNormal(math3d.vector(0, 1, 0))
	si:setNormalDirectionSize(0)
	si:setRadius(0.2)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, 0.1, 0))
		
	-- emitter
	local e = f:emitter()
	e:setPPS(500/width)	
	e:setIniter(f:initer({ bi, si, di }))
	pnode:addEmitter(e, delay, delay+1)
	
	-- renderer
	local r = f:renderer("Quad")
	r:setOrintation(math3d.vector(0, 0, 1), math3d.vector(1, 0, 0))
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath("kaze.jpg"))	
	
	-- affector
	local a = f:affector("Rotation")
	a:setPivot(math3d.vector())
	a:setSpeed(math3d.vector(0, 1000/math.sqrt(width/2), 0))
	pnode:addAffector(a)
	
	Utils.addTransparentAffector(pnode, f, 0.5)
end

local function addSand(pnode, f, scale, delay, pos, radius, len)
	pnode:setPosition(pos)
	
	local width = 0.5 * scale
	local height = width/5*3
	
	-- intier
	local bi = f:initer("Basic")
	local life = 3
	bi:setColor(Color.new(0xffd47f11))
	bi:setLifeTime(life)
	bi:setWidth(width, width*2)
	bi:setHeight(height, height*2)
	
	local si = f:initer("Cylinder")
	si:setLength(len)
	si:setNormal(math3d.vector(0, 1, 0))
	si:setNormalDirectionSize(0)
	si:setRadius(radius)
	
	local di = f:initer("Direction")
	di:setMaxAngleDegree(0)
	di:setDirection(math3d.vector(0, 0.1, 0))
		
	-- emitter
	local e = f:emitter()
	e:setPPS(50)	
	e:setIniter(f:initer({ bi, si, di }))
	pnode:addEmitter(e, delay, delay+0.1)
	
	-- renderer
	local r = f:renderer("Billboard")
	Utils.addEquilRegion(r, 1, 5)
	pnode:setRenderer(r)
	
	Utils.setMaterial(r:getMaterial())
	pnode:setRendererTexture(r, 0, Utils.getImagePath("leaf.png"))
	
	local tornadoStop = 1.2/life
	local downtime = 1.8/life
	local a = f:affector("Rotation")
	a:setPivot(math3d.vector())
	a:setSpeed(math3d.vector(0, 1000/math.sqrt(radius/2), 0))
	pnode:addAffector(a, 0, tornadoStop)
	
	for i=1,10 do
		local a = f:affector("Rotation")
		a:setPivot(math3d.vector())
		a:setSpeed(math3d.vector(0, (1000 - 100*(i-1))/math.sqrt(radius/2), 0))
		pnode:addAffector(a, tornadoStop + (i-1)/10*(downtime-tornadoStop), tornadoStop + i/10*(downtime-tornadoStop))
	end
	
	local a = f:affector("Force")
	a:setGravity(math3d.vector(0, -5, 0))
	a:setForce(math3d.vector(0, 0, 0))
	pnode:addAffector(a, downtime - 0.1)
	
	Utils.addTransparentAffector(pnode, f, downtime)
end


return function(pnode, f, env, scale)	
----------------------------------------------
	scale = scale or 1
	local tar = env:getNode("@target")
	pnode:setPosition(env:getPosition(tar))
	
	local timeGap = 0.05
	local log2 = function (num)
		return math.log(num)/math.log(2)
	end

	local factor = 25*scale/8
	for i=1,8 do
		local pos = math3d.vector(0, log2(i)*factor, 0)
		local radius = i*scale/2
		local len = (log2(i+1) - log2(i))*factor
		addWind(pnode:newChild(), f, (i-1)*timeGap, pos, radius*2, len)
		addSand(pnode:newChild(), f, scale, (i-1)*timeGap, pos, radius, len)
	end
	
----------------------------------------------
	env:deferMessage(6*timeGap, "attack begin")
	env:deferMessage(1.5, "attack end")
end

