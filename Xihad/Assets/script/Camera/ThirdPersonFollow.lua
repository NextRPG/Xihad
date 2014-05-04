local base = require 'Action.Action'
local Algorithm = require 'std.Algorithm'

local ThirdPersonFollow = {
	k = 2,
	constant = 10,	-- TODO step function
	avoidRadius = 30,
	freeRadius  = 35,	
	declineRadius = 35,
	
	_state = 'focusing',
	_following= nil,
	_follower = nil,
	_lastFollowingPosition = nil,
	_baseHeight = nil,
	
	_listeners = nil,
}
ThirdPersonFollow.__index = ThirdPersonFollow
setmetatable(ThirdPersonFollow, base)

function ThirdPersonFollow.new(follower, baseHeight)
	local obj = setmetatable(base.new(), ThirdPersonFollow)
	
	obj._follower = follower
	if not baseHeight then
		local _, y, _ = obj._follower:getTranslate():xyz()
		baseHeight = y
	end
	
	obj._baseHeight = baseHeight
	obj._listeners = {}
	
	return obj
end

-- TODO 接口化
local function toGround(vector)
	vector:set(nil, 0, nil)
	return vector
end

function ThirdPersonFollow:addFocusListener(l)
	if not l then return end
	self._listeners[l] = true
end

function ThirdPersonFollow:removeFocusListener(l)
	self._listeners[l] = nil
end

function ThirdPersonFollow:setFollowing(following)
	self._following= following
	
	if following then
		self._lastFollowingPosition = self:_getFollowingPos()
		self._state = 'focusing'
	end
end

function ThirdPersonFollow:_getFollowingPos()
	return toGround(self._following:getTranslate())
end

function ThirdPersonFollow:_getFollowerPos()
	return toGround(self._follower:getTranslate())
end

function ThirdPersonFollow:_getToFollowing()
	local sourcePosition = self:_getFollowerPos()
	local targetPosition = self:_getFollowingPos()
	
	return targetPosition - sourcePosition
end

function ThirdPersonFollow:_getDistance()
	return self:_getToFollowing():length()
end

function ThirdPersonFollow:_getFollowingVelocity()
	local vel = self:_getFollowingPos() - self._lastFollowingPosition
	vel:normalize()
	return vel
end

function ThirdPersonFollow:_computeHeight(distance)
	distance = Algorithm.clamp(distance, self.freeRadius, self.declineRadius)
	return math.sqrt(self._baseHeight^2 + self.freeRadius^2 - distance^2)
end

function ThirdPersonFollow:_adjustHeight(distance)
	local x, _, z = self._follower:getTranslate():xyz()
	self._follower:resetTranslate(math3d.vector(x, self:_computeHeight(distance), z))
end

function ThirdPersonFollow:onUpdate(time)
	if not self._following then
		return
	end
	
	local followerVel
	local distance = self:_getDistance()
	
	if self._state == 'focusing' then
		followerVel = self:_getToFollowing()
		
		local expectedDistance = self.avoidRadius + (self.freeRadius - self.avoidRadius) / 2
		local difference = distance - expectedDistance
		local step
		if difference > 0 then
			step = (difference * math.abs(self.k) + math.abs(self.constant))*time
		else
			step = (difference * math.abs(self.k) - math.abs(self.constant))*time
		end
		if math.abs(difference) < step then
			step = difference
			self._state = 'chasing'
			for listener, _ in pairs(self._listeners) do
				listener(self)
			end
		end
		
		followerVel:setLength(step)
	else -- chasing state
		if distance < self.avoidRadius then
			local followingVel = self:_getFollowingVelocity()
			followerVel = followingVel:cross(math3d.vector(0, 1, 0))
			
			if distance == 0 then
				followerVel = followerVel * self.avoidRadius
			else
				local toFollowing = self:_getToFollowing()
				if followerVel:dot(toFollowing) > 0 then
					followerVel = -followerVel
				end
				
				-- According to `Law of cosines`
				local a = 1
				local b = -2 * (toFollowing:dot(followerVel))
				local c = toFollowing:length2() - self.avoidRadius*self.avoidRadius
				assert((b^2 - 4*a*c) > 0)
				local x = (-b + math.sqrt(b*b-4*a*c)) / (2*a)
				assert(x > 0)
				followerVel = followerVel * x
			end
		elseif distance > self.declineRadius then
			followerVel = self:_getToFollowing()
			local advance = followerVel:length() - self.declineRadius
			followerVel:setLength(advance)
		end
	end
	
	if followerVel then
		self._follower:concatTranslate(followerVel)
	end
	
	self:_adjustHeight(distance)
	self._lastFollowingPosition = self:_getFollowingPos()
end

return ThirdPersonFollow
