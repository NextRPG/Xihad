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

function Utils.getImagePath(img)
	local AssetsPath = "../Xihad/Assets/gfx/"
	return AssetsPath..img
end

function Utils.colorMul(color, ratio)
	return Color.new(0xff, ratio*color:getRed(), ratio*color:getGreen(), ratio*color:getBlue())
end

function Utils.colorAdd(color, offset)
	return Color.new(0xff, color:getRed() + offset, color:getGreen() + offset, color:getBlue() + offset)
end

function Utils.setMaterial(mat, blendSrc, blendDst, blendOp, materialType, ZEnable)
	blendSrc = blendSrc or "src.alpha"
	blendDst = blendDst or "1-src.alpha"
	blendOp = blendOp or "add"
	materialType = materialType or "trans_add"
	ZEnable = ZEnable or true
	
	mat:setBlend(blendSrc, blendSrc, blendOp)
	mat:setMaterialType(materialType)
	mat:setLighting(false)
	mat:setZWriteEnable(ZEnable)
end
return Utils