require 'Assets.test.boot_moveWithShadow'
require 'Assets.script.AllPackages'
local ThirdPersonFollow = require 'Camera.ThirdPersonFollow'
local ActionAdapter= require 'Action.ActionAdapter'

local ninja  = g_scene:findObject(c'ninja1')
local camera = g_scene:findObject(c'camera')
camera:concatTranslate(math3d.vector(0, 10, -100))
local _3rdCamera = ThirdPersonFollow.new(camera, ninja)
ActionAdapter.fit(camera, _3rdCamera)

camera:appendUpdateHandler({
	onUpdate = function()
		local cam = camera:findComponent(c'Camera')
		local render = ninja:findComponent(c'Render')
		cam:setTarget(ninja:getTranslate() + render:getAABB():center())
	end
})

-- camera:appendUpdateHandler{
-- 	onUpdate = function (self)
-- 		if g_time.global > 3 then
-- 			_3rdCamera:resetFollowing(g_scene:findObject(c'ninja2'))
-- 			self:stop()
-- 		end
-- 	end
-- }
