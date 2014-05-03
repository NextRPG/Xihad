local Trigonometry = {}

function Trigonometry.toDegree(rad)
	return rad * 180 / math.pi
end

function Trigonometry.toRadian(degree)
	return degree / 180 * math.pi
end

function Trigonometry.normalize(angle)
	angle = angle % 360
	if angle < -180 then
		angle = angle + 360
	elseif angle >= 180 then
		angle = angle - 360
	end
	
	assert(angle < 180 and angle >= -180)
	
	return angle
end

function Trigonometry.minDeltaAngle(degree1, degree2)
	local delta = degree1 - degree2
	return Trigonometry.normalize(delta)
end

function Trigonometry.equal(degree1, degree2)
	return (degree2 - degree1) % 360 == 0
end

return Trigonometry
