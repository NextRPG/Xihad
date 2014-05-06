local Algorithm = require 'std.Algorithm'
local XihadMapTile = require 'Chessboard.XihadMapTile'
local CameraAvoid = {
	maxShiftSpeed = 0.5,
	avoidRadius = 30,
	
	_cameraObject = nil,
	_lastFollowingPosition = nil,
}
CameraAvoid.__index = CameraAvoid

function CameraAvoid.new(camera)
	local obj = setmetatable({
			_cameraObject = camera,
			_cameraControl= camera:findComponent(c'Camera'),
		}, CameraAvoid)
	
	obj._lastFollowingPosition = obj:_getToFollowing()	
	return obj
end

function CameraAvoid:_getToFollowing()
	local dir = self._cameraControl:getLookDirection()
	dir:set(nil, 0, nil)
	return dir
end

function CameraAvoid:update(time)
	local toFollowing = self:_getToFollowing()
	local distance = toFollowing:length()
	
	if distance < self.avoidRadius then
		local followingVel = toFollowing - self._lastFollowingPosition
		local followerVel = followingVel:cross(math3d.vector(0, 1, 0))
		followerVel:normalize()
		
		if distance == 0 then
			followerVel = followerVel * self.avoidRadius
		else
			if followerVel:dot(toFollowing) > 0 then
				followerVel = -followerVel
			end
			
			-- According to `Law of cosines`
			local a = 1
			local b = -2 * (toFollowing:dot(followerVel))
			local c = distance*distance - self.avoidRadius*self.avoidRadius
			assert((b^2 - 4*a*c) > 0)
			local x = (-b + math.sqrt(b*b-4*a*c)) / (2*a)
			assert(x > 0)
			
			followerVel = followerVel * math.min(x, self.maxShiftSpeed)
		end
		
		self._cameraObject:concatTranslate(followerVel)
	end
	
	self._lastFollowingPosition = toFollowing
end

return CameraAvoid