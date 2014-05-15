local base = require 'Modifier.Modifier'
local Trigonometry = require 'math.Trigonometry'
local DirectAvoid = {
	maxSpeed = 1200,		-- 120/s
	tangent  = nil,
	
	follower = nil,
	following= nil,
	_lastFollowingPos = nil,
}
DirectAvoid.__index = DirectAvoid
setmetatable(DirectAvoid, base)

function DirectAvoid.new(follower, following, elevation)
	local o = setmetatable({
			follower = follower,
			following= following,
			_lastFollowingPos = following:getTranslate(),
		}, DirectAvoid)
	
	o:setAvoidElevation(elevation or 45)
	return o
end

function DirectAvoid:_toGround(vec)
	vec:set(nil, 0, nil)
	return vec
end

function DirectAvoid:setAvoidElevation(degree)
	self.tangent = math.tan(Trigonometry.toRadian(degree))
end

function DirectAvoid:_getFollowingPos()
	return self.following:getTranslate()
end

function DirectAvoid:_getToFollowing()
	local followerPosition  = self.follower:getTranslate()
	local followingPosition = self:_getFollowingPos()
	return self:_toGround(followingPosition - followerPosition)
end

function DirectAvoid:_getFollowerVelDir(toFollowing)
	local followingVel= self:_getFollowingPos() - self._lastFollowingPos
	local followerDir = followingVel:cross(math3d.vector(0, 1, 0))
	followerDir:normalize()
	
	if followerDir:dot(toFollowing) > 0 then
		followerDir = -followerDir
	end
	
	return followerDir
end

function DirectAvoid:_getAvoidRadius2()
	local _, height = self.follower:getTranslate():xyz()
	local radius = height * self.tangent
	return radius*radius
end

function DirectAvoid:_getFollowerVelSize(toFollowing, followerDir, time)
	---
	-- According to `Law of cosines`
	-- and then resolve a*x^2 + b*x + c = 0
	local a = 1
	local b = -2 * (toFollowing:dot(followerDir))
	local c = toFollowing:length2() - self:_getAvoidRadius2()
	local delta = b*b - 4*a*c
	assert(delta >= 0)
	
	local x = (-b + math.sqrt(delta)) / (2*a)
	assert(x > 0)
	
	return math.min(DirectAvoid.maxSpeed * time, x)
end

function DirectAvoid:_tooClose(toFollowing)
	return toFollowing:length2() < self:_getAvoidRadius2()
end

function DirectAvoid:onUpdate(time)
	local toFollowing = self:_getToFollowing()
	if self:_tooClose(toFollowing) then
		local velDir = self:_getFollowerVelDir(toFollowing)
		local velSize = self:_getFollowerVelSize(toFollowing, velDir, time)
		self.follower:concatTranslate(velDir * velSize)
	end
	
	self._lastFollowingPos = self:_getFollowingPos()
end

return DirectAvoid