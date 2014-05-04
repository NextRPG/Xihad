local Trigonometry = {
	RADIAN_TO_DEGREE = 180 / math.pi,
	DEGREE_TO_RADIAN = math.pi / 180,
}

function Trigonometry.toDegree(rad)
	return rad * Trigonometry.RADIAN_TO_DEGREE
end

function Trigonometry.toRadian(degree)
	return degree * Trigonometry.DEGREE_TO_RADIAN
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

function Trigonometry.equal(degree1, degree2)
	return (degree2 - degree1) % 360 == 0
end

return Trigonometry
