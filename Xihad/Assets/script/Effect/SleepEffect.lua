--- 
-- 使得人物不能移动几回合
-- @module SleepEffect
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local BaseEffect = require "BaseEffect"

local SleepEffect = {

}

function SleepEffect.new( o, object )
	inherit(o, SleepEffect, BaseEffect)
	o:init(object)
	return o
end

function SleepEffect:roundUpdate( currentTeam )
	if self:checkUpdate( currentTeam ) then
		self.target.states.TURNOVER = true
	end
end


return SleepEffect