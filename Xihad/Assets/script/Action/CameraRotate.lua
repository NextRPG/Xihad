--- 
-- 让相机绕着某个轴旋转
-- @module CameraRotate
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraRotate = {

}

function CameraRotate.new( o )
	o = o or {}
	setmetatable(o, {__index = CameraRotate})

	return o
end

function CameraRotate:start( axis )
	self.axis = axis
	self.enabled = true
end

function CameraRotate:stop(  )
	self.axis = nil
	self.enabled = false
end

function CameraRotate:onUpdate(  )
	if not self.enabled then return false end
	
	local camera = self.object
	local ccom = self.object:findComponent(c"Camera")
	local q = math3d.quaternion(1, self.axis)

	local orientation = camera:getTranslate() - ccom:getTarget()
	orientation = q * orientation
	camera:resetTranslate(ccom:getTarget() + orientation)

end

return CameraRotate