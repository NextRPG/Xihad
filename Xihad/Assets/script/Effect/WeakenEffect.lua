--- 
-- 造成对敌人的属性削弱效果
-- @module WeakenEffect
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local BaseEffect = require "BaseEffect"
local BuffEffect = require "BuffEffect"

local WeakenEffect = {
	
}

function WeakenEffect.new( o, object )
	inherit(o, WeakenEffect, BaseEffect)
	o:init( object )
	o.onStop_aux = BuffEffect.onStart
	o.onStart_aux = BuffEffect.onStop

	return o
end

function WeakenEffect:onStart(  )
	self:onStart_aux()
	print("physicalAttack is", self.target:getProperty("physicalAttack"))
end

function BuffEffect:roundUpdate( currentTeam )
	self:checkUpdate( currentTeam ) 
end

function WeakenEffect:onStop(  )
	self:onStop_aux()
end

return WeakenEffect