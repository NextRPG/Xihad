local width = Consts.TILE_WIDTH
local height = Consts.TILE_HEIGHT

function point2vector( point )
	return math3d.vector(point.x * width, 0, point.y * height)
end

function getLogicLocation( location )
	return {x = math.floor(location.x / width) , y = math.floor(location.z / height)}
end

function getPixelLocation( location )
	return 	{ x = (location.x * width) , 
			z = (location.y * height) } 
end

-- 对PixelLocation进行再加工
function getRoundPixelLocation( location )
	return  math.round(location.x / width) * width, math.round(location.z / height) * height 
end

function getLogicAngle( location )
	local x, y = location.x, location.y
	if x > 0 then return 90
	elseif x < 0 then return -90
	elseif y < 0 then return 180
	else return 0
	end
end

function calRotation( currentRotation, targetRotation )
	-- print("rotation", targetRotation, currentRotation)
	if (targetRotation - currentRotation) < -180 then
		return 360 + targetRotation - currentRotation
	elseif (targetRotation - currentRotation) > 180 then
		return targetRotation - currentRotation - 360
	else
		return targetRotation - currentRotation
	end
	-- return targetRotation - currentRotation
end

-- MouseEvent

function getSelectedObject(  )
	return collision:detect(
		collision:getRayFromScreenCoord(cursor:getPosition()))
end

function isSelectedObject( id )
	assert(id, "id can't be nil")
	return collision:detect(
		collision:getRayFromScreenCoord(cursor:getPosition()), id)
end
