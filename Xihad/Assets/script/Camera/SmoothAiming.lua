local Algorithm = require 'std.Algorithm'
local Trigonometry = require 'math.Trigonometry'
local SmoothAiming = {
	anglesPerSecond = 90,	-- TODO step function
	elevation = 70,
	
	_lookDir = nil,
	_aimObject = nil,
	_aabbCenter= nil,
	_cameraControl= nil,
	_cameraObject = nil,
	
	_listeners = nil,
	_state = 'aiming',
}
SmoothAiming.__index = SmoothAiming

function SmoothAiming.new(cameraObject)
	local o = setmetatable({
			_cameraObject = cameraObject,
			_cameraControl= cameraObject:findComponent(c'Camera'),
			_listeners = {},
		}, SmoothAiming)
	
	o:_setAim(nil)
	return o
end

function SmoothAiming:update(time)
	local sourceLookDir = self._cameraControl:getLookDirection()
	sourceLookDir:normalize()
	
	local targetLookDir
	if not self._aimObject then 
		targetLookDir = self._lookDir:copy()
	else
		local center = self._aimObject:getTranslate() + self._aabbCenter
		targetLookDir = center - self._cameraObject:getTranslate()
		targetLookDir:normalize()
	end
	
	if sourceLookDir == targetLookDir then
		assert(self._state ~= 'aiming')
		return 
	end
	
	local nextLookDir
	if self._state == 'aimed' then
		nextLookDir = targetLookDir
	else
		local step = self.anglesPerSecond * time
		local rad  = math.acos(Algorithm.clamp(sourceLookDir:dot(targetLookDir), -1, 1))
		local angle= Trigonometry.toDegree(rad)
		
		if angle >= -step and angle <= step then
			nextLookDir = targetLookDir
			
			if self._state == 'aiming' then
				-- TODO
				for lis, _ in pairs(self._listeners) do
					lis(self)
				end
				self._state = 'aimed'
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
	
	if aimObject then
		local render = aimObject:findComponent(c'Render')
		self._aabbCenter = render:getAABB():center()
	else
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
