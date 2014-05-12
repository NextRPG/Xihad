local base = require 'Modifier.Modifier'
local CameraAvoid = {
	maxSpeed = 120,		-- 120/s
	avoidRadius = 30,
	
	follower = nil,
	following= nil,
	_lastFollowingPos = nil,
}
CameraAvoid.__index = CameraAvoid
setmetatable(CameraAvoid, base)

function CameraAvoid.new(follower, following)
	return setmetatable({
			follower = follower,
			following= following,
			_lastFollowingPos = following:getTranslate(),
		}, CameraAvoid)
end

function CameraAvoid:_toGround(vec)
	vec:set(nil, 0, nil)
	return vec
end

function CameraAvoid:_getFollowingPos()
	return self.following:getTranslate()
end

function CameraAvoid:_getToFollowing()
	local followerPosition  = self.follower:getTranslate()
	local followingPosition = self:_getFollowingPos()
	return self:_toGround(followingPosition - followerPosition)
end

function CameraAvoid:_getFollowerVelDir(toFollowing)
	local followingVel= self:_getFollowingPos() - self._lastFollowingPos
	local followerDir = followingVel:cross(math3d.vector(0, 1, 0))
	followerDir:normalize()
	
	if followerDir:dot(toFollowing) > 0 then
		followerDir = -followerDir
	end
	
	return followerDir
end

function CameraAvoid:_getFollowerVelSize(toFollowing, followerDir, time)
	---
	-- According to `Law of cosines`
	-- and then resolve a*x^2 + b*x + c = 0
	local a = 1
	local b = -2 * (toFollowing:dot(followerDir))
	local c = toFollowing:length2() - self.avoidRadius*self.avoidRadius
	local delta = b*b - 4*a*c
	assert(delta >= 0)
	
	local x = (-b + math.sqrt(delta)) / (2*a)
	assert(x > 0)
	
	return math.min(CameraAvoid.maxSpeed * time, x)
end

function CameraAvoid:_tooClose(toFollowing)
	local avoidRadius = self.avoidRadius
	return toFollowing:length2() < avoidRadius*avoidRadius
end

function CameraAvoid:onUpdate(time)
	local toFollowing = self:_getToFollowing()
	if self:_tooClose(toFollowing) then
		local velDir = self:_getFollowerVelDir(toFollowing)
		local velSize = self:_getFollowerVelSize(toFollowing, velDir, time)
		self.follower:concatTranslate(velDir * velSize)
	end
	
	self._lastFollowingPos = self:_getFollowingPos()
end

return CameraAvoid