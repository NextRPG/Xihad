--- 
-- 检查战斗是否胜利
-- @module VictoryChecker
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG
local Listener = require "Listener"
local BattleManager = require "BattleManager"

local VictoryChecker = {}

function VictoryChecker:init(  )
	local dispatcher = g_scene:getDispatcher()
	dispatcher:addListener("AI.die", self)
	dispatcher:addListener("Hero.die", self)
	self:drop()
	inherit(self, Listener)
end

function VictoryChecker:onHeroDie( srcObject, param )
	if not g_scene:hasObjectWithTag("Hero") then
		print("AI won")
		BattleManager.stateMachine:stop()
	end
end

function VictoryChecker:onAIDie( srcObject, param )
	if not g_scene:hasObjectWithTag("AI") then
		print("Hero won")
		BattleManager.stateMachine:stop()
	end
end

return VictoryChecker