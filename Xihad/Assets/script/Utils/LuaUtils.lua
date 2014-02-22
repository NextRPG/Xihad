function createEnumTable( t, index )
	local enumTable = {}
	local enumIndex = index or 0
	for i,v in ipairs(t) do
		enumTable[v] = enumIndex + i
	end
	return enumTable
end

-- coordinate

local width = Consts.TILE_WIDTH
local height = Consts.TILE_HEIGHT

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

-- table
function table.copy( t )
	local t_copy = {}
	for k,v in pairs(t) do
		t_copy[k] = v
	end
	return t_copy
end

function table.equal( t1, t2 )
	for k,v in pairs(t1) do
		if v ~= t2[k] then return false end
	end
	return true
end

function table.reverse( t )
	local i = 1
	while i < (#t + 1 - i) do
		t[i],t[#t + 1 - i] = t[#t + 1 - i],t[i]
		i = i + 1
	end
end

-- math

function math.round( i )
	return i - math.floor(i) > 0.5 and math.ceil(i) or math.floor(i)
end

function table.contains( t, e )
	for k,v in pairs(t) do
		if v == e then return true end
		if table.equal(v, e) then return true end
	end
	return false
end

-- function

function runAsyncFunc( func, ... )
	local current = coroutine.running()
	local t = {...}
	table.insert(t, function (  )
		local result, func, t = coroutine.resume(current)
		print(result, func, t)
		if type(func) == "function" then
			func(unpack(t))
		end
	end)
	coroutine.yield(func, t)
end