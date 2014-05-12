local base = require 'Modifier.Modifier'
local Trigonometry = require 'math.Trigonometry'
local SmoothAiming = {
	angularVel= 90, -- 角速度
	elevation = 60,	-- 仰角
	
	_lookDir  = nil,
	_aimObject= nil,
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
end

function SmoothAiming:_checkAiming()
	if self._state == 'aiming' then
		self:_fireAimed()
		self._state = 'aimed'
	end
end

function SmoothAiming:_getSourceLookDir()
	local sourceLookDir = self._cameraControl:getLookDirection()
	sourceLookDir:normalize()
	return sourceLookDir
end

function SmoothAiming:_getFinishLookDir()
	local finishLookDir
	if not self._aimObject then 
		finishLookDir = self._lookDir
	else
		local center = self._aimObject:getTranslate()
		finishLookDir = center - self._cameraObject:getTranslate()
		finishLookDir:normalize()
	end
	
	return finishLookDir
end

function SmoothAiming:_getTarget(lookDir)
	local XihadMapTile = require 'Chessboard.XihadMapTile'
	local source = self._cameraObject:getTranslate()
	local ray = math3d.line(source, source + lookDir)
	return XihadMapTile.intersectsGround(ray)
end

function SmoothAiming:onUpdate(time)
	local sourceLookDir = self:_getSourceLookDir()
	local finishLookDir = self:_getFinishLookDir()
	local nextLookDir = finishLookDir
	if sourceLookDir ~= finishLookDir and self._state ~= 'aimed' then
		local step = math.abs(self.angularVel * time)
		local angle= Trigonometry.getAngleBetweenN(sourceLookDir, finishLookDir)
		
		-- angle >= 0 and angle <= 180
		-- step  >= 0
		if angle > step then
			local axis = sourceLookDir:cross(finishLookDir)
			nextLookDir = math3d.quaternion(step, axis) * sourceLookDir
		end
	end
	
	self._cameraControl:setTarget(self:_getTarget(nextLookDir))
	if nextLookDir == finishLookDir then
		self:_checkAiming()
	end
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
