local base = require 'Action.Action'
local Algorithm = require 'std.Algorithm'
local Trigonometry = require 'math.Trigonometry'
local SmoothAiming = {
	anglesPerSecond = 90,	-- TODO step function

	_lookDir = nil,
	_aimObject = nil,
	_aabbCenter= nil,
	_cameraControl= nil,
	_cameraObject = nil,	
}
SmoothAiming.__index = SmoothAiming
setmetatable(SmoothAiming, base)

function SmoothAiming.new(cameraObject, lookDir)
	local o = setmetatable(base.new(), SmoothAiming)
	
	o._lookDir = lookDir:copy()
	o._lookDir:normalize()
	
	o._cameraObject = cameraObject
	o._cameraControl= cameraObject:findComponent(c'Camera')
	return o
end

function SmoothAiming:onUpdate(time)
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
		return 
	end
	
	local step = self.anglesPerSecond * time
	local rad  = math.acos(Algorithm.clamp(sourceLookDir:dot(targetLookDir), -1, 1))
	local angle = Trigonometry.toDegree(rad)
	local nextLookDir
	if angle > -step and angle < step then
		nextLookDir = targetLookDir
	else
		if angle < -step then
			step = -step 
		end
		
		local axis = sourceLookDir:cross(targetLookDir)
		local q = math3d.quaternion(step, axis)
		nextLookDir = q * sourceLookDir
	end
	
	-- look dir must have a large length
	nextLookDir:setLength(10000)
	self._cameraControl:setLookDirection(nextLookDir)
end

-- function SmoothAiming:addAimListener(lis)
-- end

-- function SmoothAiming:removeAimListener(lis)
-- end

function SmoothAiming:setAim(aimObject)
	print('set aim: ', aimObject)
	self._aimObject = aimObject
	
	if aimObject then
		local render = aimObject:findComponent(c'Render')
		self._aabbCenter = render:getAABB():center()
	end
end

return SmoothAiming
