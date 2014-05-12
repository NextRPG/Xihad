local base = require 'Modifier.Modifier'
local Algorithm = require 'std.Algorithm'
local ThirdPersonFollow = {
	k = 3,
	constant = 5,	-- TODO step function
	avoidRadius = 30,
	freeRadius  = 45,
	declineRadius = 45,
	
	_state = 'focusing',
	_following= nil,
	_follower = nil,
	_baseHeight = nil,
	
	_listeners = nil,
}
ThirdPersonFollow.__index = ThirdPersonFollow
setmetatable(ThirdPersonFollow, base)

function ThirdPersonFollow.new(follower, baseHeight)
	local obj = setmetatable({
			_follower = follower,
			_listeners = {},
		}, ThirdPersonFollow)
	
	if not baseHeight then
		local _, y, _ = obj._follower:getTranslate():xyz()
		baseHeight = y
	end
	
	obj._baseHeight = baseHeight
	
	return obj
end

-- TODO 接口化
local function toGround(vector)
	vector:set(nil, 0, nil)
	return vector
end

function ThirdPersonFollow:hasFocused()
	return self._state ~= 'focusing'
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
		
		if self:_getDistance() > self.declineRadius then
			self._state = 'focusing'
		else
			self._state = 'chasing'
		end
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
	if not self._following then return end
	
	local followerVel
	local distance = self:_getDistance()
	
	if self._state == 'focusing' then
		followerVel = self:_getToFollowing()
		
		local expectedDistance = self.freeRadius -- self.avoidRadius + (self.freeRadius - self.avoidRadius) / 2
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
		if distance > self.declineRadius then
			followerVel = self:_getToFollowing()
			local advance = followerVel:length() - self.declineRadius
			followerVel:setLength(advance)
		end
	end
	
	if followerVel then
		self._follower:concatTranslate(followerVel)
	end
	
	self:_adjustHeight(distance)
end

return ThirdPersonFollow
