require 'Assets.test.boot_moveWithShadow'
require 'Assets.script.AllPackages'
local DirectFollow = require 'Camera.DirectFollow'
local ModifierAdapter= require 'Modifier.ModifierAdapter'
local SmoothAiming = require 'Camera.SmoothAiming'
local DirectAvoid = require 'Camera.DirectAvoid'

local ninja  = g_scene:findObject(c'ninja1')
local camera = g_scene:findObject(c'camera')
camera:concatTranslate(math3d.vector(0, 10, -100))
local followControl = DirectFollow.new(camera)
local aimingControl = SmoothAiming.new(camera, math3d.vector(0.5, -0.5, 0.5))
ModifierAdapter.fit(camera, followControl)
ModifierAdapter.fit(camera, aimingControl)
ModifierAdapter.fit(camera, DirectAvoid.new(camera))
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
