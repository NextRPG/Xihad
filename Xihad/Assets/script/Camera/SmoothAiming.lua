local base = require 'Modifier.Modifier'
local Algorithm = require 'std.Algorithm'
local Trigonometry = require 'math.Trigonometry'
local SmoothAiming = {
	lastStep= 0,
	maxStep = 120,
	minStep = 45,
	deceleration = 40,
	acceleration = 240,
	uniformMotionTime = 0.2,
	
	elevation = 70,
	
	_lookDir = nil,
	_aimObject = nil,
	_cameraControl= nil,
	_cameraObject = nil,
	
	_listeners = nil,
	_state = 'aiming',
}
SmoothAiming.__index = SmoothAiming
setmetatable(SmoothAiming, base)

function SmoothAiming.new(cameraObject)
	local o = setmetatable({
			_cameraObject = cameraObject,
			_cameraControl= cameraObject:findComponent(c'Camera'),
			_listeners = {},
		}, SmoothAiming)
	
	o:_setAim(nil)
	return o
end

function SmoothAiming:_fireAimed()
	for lis, _ in pairs(self._listeners) do
		lis(self)
	end
	
	self._state = 'aimed'
end

function SmoothAiming:onUpdate(time)
	local sourceLookDir = self._cameraControl:getLookDirection()
	sourceLookDir:normalize()
	
	local targetLookDir
	if not self._aimObject then 
		targetLookDir = self._lookDir
	else
		local center = self._aimObject:getTranslate()
		targetLookDir = center - self._cameraObject:getTranslate()
		targetLookDir:normalize()
	end
	
	if sourceLookDir == targetLookDir then
		if self._state == 'aiming' then
			self:_fireAimed()
		end
		
		self.lastStep = 0
		return 
	end
	
	local nextLookDir
	if self._state == 'aimed' then
		nextLookDir = targetLookDir
	else
		local angle = Trigonometry.getAngleBetweenN(sourceLookDir, targetLookDir)
		
		local step 
		local uniformThreshold = self.minStep * self.uniformMotionTime
		if angle <= uniformThreshold then
			 step = self.minStep
		else
			local deceleratedDistance = angle - uniformThreshold
			step = math.sqrt(deceleratedDistance * 2 * math.abs(self.deceleration) + self.minStep*self.minStep)
			
			assert(step > self.minStep, step)
			step = math.min(self.maxStep, step)
		end
		
		local delta = math.abs(self.acceleration) * time
		step = Algorithm.clamp(step, self.lastStep-delta, self.lastStep+delta)
		step = step * time
		
		if angle >= -step and angle <= step then
			nextLookDir = targetLookDir
			
			if self._state == 'aiming' then
				self:_fireAimed()
			end
		else
			if angle < -step then
				step = -step
			end

			local axis = sourceLookDir:cross(targetLookDir)
			local q = math3d.quaternion(step, axis)
			nextLookDir = q * sourceLookDir
		end
	end
	
	self.lastStep = Trigonometry.getAngleBetween(sourceLookDir, nextLookDir)
	if time ~= 0 then
		self.lastStep = self.lastStep / time
	end
	
	local XihadMapTile = require 'Chessboard.XihadMapTile'
	local source = self._cameraObject:getTranslate()
	local ray = math3d.line(source, source + nextLookDir)
	local point = XihadMapTile.intersectsGround(ray)
	
	self._cameraControl:setTarget(point)
end

function SmoothAiming:addAimListener(lis)
	self._listeners[lis] = true
end

function SmoothAiming:removeAimListener(lis)
	self._listeners[lis] = nil
end

function SmoothAiming:_setAim(aimObject)
	self._aimObject = aimObject
	self._state = 'aiming'
	
	if not aimObject then
		local currentLookDir = self._cameraControl:getLookDirection()
		local downVector = -self._cameraControl:getUpVector()
		local axis = downVector:cross(currentLookDir)
		self._lookDir = math3d.quaternion(self.elevation, axis) * downVector
		self._lookDir:normalize()
	end
end

function SmoothAiming:setAim(aimObject)
	if self._aimObject ~= aimObject then
		self:_setAim(aimObject)
	end
end

return SmoothAiming
