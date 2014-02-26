--- 
-- 让相机跟随某个Object进行移动
-- @module CameraFollow
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local CameraFollow = {

}

function CameraFollow.new( o )
	o = o or {}
	setmetatable(o, {__index = CameraFollow})

	return o
end

function CameraFollow:start( object )

	self.follow = object
	self.enabled = true

end

function CameraFollow:stop(  )
	
	self.follow = nil
	self.enabled = false

end

function CameraFollow:onUpdate(  )
	if not self.enabled then return false end

	local camera = self.object
	local ccom = self.object:findComponent(c"Camera")
	local delta = self.follow:getTranslate() - ccom:getTarget()
	camera:concatTranslate(delta)
	ccom:setTarget(self.follow:getTranslate())

end


return CameraFollow