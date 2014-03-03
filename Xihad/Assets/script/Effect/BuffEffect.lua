--- 
-- 造成对我方的增益效果
-- @module BuffEffect
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local BaseEffect = require "BaseEffect"

local BuffEffect = {

}

function BuffEffect.new( o, object )
	inherit(o, BuffEffect, BaseEffect)
	o:init( object )

	return o
end

function BuffEffect:onStart(  )
	for k,equation in pairs(self.target.properties) do
		if self[k .. "Power"] ~= nil then
			equation:addPower(self[k .. "Power"])
		end
		if self[k .. "Fixed"] ~= nil then
			equation:addFixed(self[k .. "Fixed"])
		end
	end
end

function BuffEffect:roundUpdate( currentTeam )
	self:checkUpdate( currentTeam ) 
end

function BuffEffect:onStop(  )
	for k,equation in pairs(self.target.properties) do
		if self[k .. "Power"] ~= nil then
			equation:reducePower(self[k .. "Power"])
		end
		if self[k .. "Fixed"] ~= nil then
			equation:reduceFixed(self[k .. "Fixed"])
		end
	end
end

return BuffEffect