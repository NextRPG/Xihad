require 'Assets.test.boot_moveWithShadow'
require 'Assets.script.AllPackages'
local ThirdPersonFollow = require 'Camera.ThirdPersonFollow'
local ActionAdapter= require 'Action.ActionAdapter'
local SmoothAiming = require 'Camera.SmoothAiming'
local CameraAvoid = require 'Camera.CameraAvoid'

local ninja  = g_scene:findObject(c'ninja1')
local camera = g_scene:findObject(c'camera')
camera:concatTranslate(math3d.vector(0, 10, -100))
local followControl = ThirdPersonFollow.new(camera)
local aimingControl = SmoothAiming.new(camera, math3d.vector(0.5, -0.5, 0.5))
ActionAdapter.fit(camera, followControl)
ActionAdapter.fit(camera, aimingControl)
ActionAdapter.fit(camera, CameraAvoid.new(camera))
aimingControl:setAim(ninja)
followControl:setFollowing(ninja)

-- camera:appendUpdateHandler{
-- 	onUpdate = function (self)
-- 		if g_time.global > 3 then
-- 			_3rdCamera:resetFollowing(g_scene:findObject(c'ninja2'))
-- 			self:stop()
-- 		end
-- 	end
-- }
