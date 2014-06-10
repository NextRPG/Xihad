local base = require 'Modifier.Modifier'
local Trigonometry = require 'math.Trigonometry'
local DirectFollow = {
	tangent = 1,
	
	_following = nil,
	_follower  = nil,
}
DirectFollow.__index = DirectFollow
setmetatable(DirectFollow, base)

function DirectFollow.new(follower, freeElevation)
	local obj = setmetatable({
			_follower = follower,
			_listeners = {},
		}, DirectFollow)
	
	freeElevation = freeElevation or 55
	obj:setFreeElevation(freeElevation)
	return obj
end

function DirectFollow:setFreeElevation(degree)
	self.tangent = math.tan(Trigonometry.toRadian(degree))
end

function DirectFollow:_toGround(vector)
	vector:set(nil, 0, nil)
	return vector
end

function DirectFollow:setFollowing(following)
	self._following= following
end

function DirectFollow:_getFollowingPos()
	return self:_toGround(self._following:getTranslate())
end

function DirectFollow:_getFollowerPos()
	return self:_toGround(self._follower:getTranslate())
end

function DirectFollow:_getToFollowing()
	return self:_getFollowingPos() - self:_getFollowerPos()
end

function DirectFollow:_getDistance()
	return self:_getToFollowing():length()
end

function DirectFollow:getFreeRadius()
	local _, height = self._follower:getTranslate():xyz()
	return self.tangent * height
end

function DirectFollow:onUpdate(time)
	if not self._following then return end
	
	local freeRadius = self:getFreeRadius()
	if self:_getDistance() > freeRadius then
		local followerVel = self:_getToFollowing()
		local advance = followerVel:length() - freeRadius
		followerVel:setLength(advance)
		
		self._follower:concatTranslate(followerVel)
	end
end

return DirectFollow
