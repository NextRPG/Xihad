local Utils = { IniterCreator = {}}
local IniterCreator = Utils.IniterCreator

function Utils.addEquilRegion(target, rows, cols)
	local cellWidth = 1 / cols
	local cellHeight = 1 / rows
	
	for i = 1, rows do
		local y = (i-1) * cellHeight
		for j = 1, cols do
			local x = (j-1) * cellWidth
			target:addTexRegion(x, y, x+cellWidth, y+cellHeight)
		end
	end
end

function Utils.addTransparentAffector(pnode, factory, bgnPercent, endPercent)
	bgnPercent = bgnPercent or 0
	endPercent = endPercent or 1
	
	local a = factory:affector("FadeOut")
	a:setTargetColor(Color.new(0x00000000))
	a:setColorMask("argb")
	pnode:addAffector(a, bgnPercent, endPercent)
end

function Utils.Box(center, xRadius, yRadius, zRadius)
	xRadius = xRadius or 0
	yRadius = yRadius or 0
	zRadius = zRadius or 0
	local box = math3d.aabb(center)
	local x,y,z = center:xyz()
	box:addPoint(math3d.vector(x+xRadius, y+yRadius, z+zRadius))
	box:addPoint(math3d.vector(x-xRadius, y-yRadius, z-zRadius))
	return box
end

function IniterCreator.sphere(factory, directionSize, radius, outlineOnly)
	local si = factory:initer("Sphere") 
	si:setNormalDirectionSize(directionSize)
	si:setRadius(radius)
	si:setOutlineOnly(outlineOnly)
	return si;
end




return Utils