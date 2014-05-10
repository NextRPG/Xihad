local base = require 'Modifier.Modifier'
local Algorithm = require 'std.Algorithm'
local XihadMapTile = require 'Chessboard.XihadMapTile'
local CameraAvoid = {
	maxSpeed = 120,		-- 120/s
	avoidRadius = 30,
	
	_cameraObject = nil,
	_lastFollowingPosition = nil,
}
CameraAvoid.__index = CameraAvoid
setmetatable(CameraAvoid, base)

function CameraAvoid.new(camera)
	local obj = setmetatable({
			_cameraObject = camera,
			_cameraControl= camera:findComponent(c'Camera'),
		}, CameraAvoid)
	
	obj._lastFollowingPosition = obj:_getFollowingPosition()
	return obj
end

function CameraAvoid:_getFollowingPosition()
	return self._cameraControl:getTarget()
end

function CameraAvoid:_getToFollowing()
	local dir = self._cameraControl:getLookDirection()
	dir:set(nil, 0, nil)
	return dir
end

function CameraAvoid:onUpdate(time)
	local toFollowing = self:_getToFollowing()
	local distance = toFollowing:length()
	
	local followingPosition = self._cameraControl:getTarget()
	if distance < self.avoidRadius then
		local followingVel= followingPosition - self._lastFollowingPosition
		local followerVel = followingVel:cross(math3d.vector(0, 1, 0))
		followerVel:normalize()
	
		if followerVel:dot(toFollowing) > 0 then
			followerVel = -followerVel
		end
		
		---
		-- According to `Law of cosines`
		-- and then resolve a*x^2 + b*x + c = 0
		local a = 1
		local b = -2 * (toFollowing:dot(followerVel))
		local c = distance*distance - self.avoidRadius*self.avoidRadius
		local delta = b*b - 4*a*c
		assert(delta >= 0)
		
		local x = (-b + math.sqrt(delta)) / (2*a)
		assert(x > 0)
		
		followerVel = followerVel * math.min(CameraAvoid.maxSpeed * time, x)
		self._cameraObject:concatTranslate(followerVel)
	end
	
	self._lastFollowingPosition = followingPosition
end

return CameraAvoid