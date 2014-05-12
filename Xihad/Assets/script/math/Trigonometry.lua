local Algorithm = require 'std.Algorithm'
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

function Trigonometry.nearestTarget(source, target)
	return source + Trigonometry.normalize(target - source)
end

function Trigonometry.equal(degree1, degree2)
	return (degree2 - degree1) % 360 == 0
end

function Trigonometry.getAngleBetween(vec1, vec2)
	vec1 = vec1:copy()
	vec2 = vec2:copy()
	
	return Trigonometry.getAngleBetweenN(vec1, vec2)
end

function Trigonometry.getAngleBetweenN(vec1, vec2)
	vec1:normalize()
	vec2:normalize()
	local cos = Algorithm.clamp(vec1:dot(vec2), -1, 1)
	return Trigonometry.toDegree(math.acos(cos))
end

return Trigonometry
