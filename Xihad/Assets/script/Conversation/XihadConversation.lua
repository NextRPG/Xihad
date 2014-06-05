local base = require 'Conversation.InGameConversation'
local WarriorDelegate = require 'Conversation.WarriorDelegate'
local RegionDeterminer= require 'Conversation.RegionDeterminer'
local AsConditionFactory = require 'Async.AsConditionFactory'
local XihadConversation = setmetatable({}, base)
XihadConversation.__index = XihadConversation

function XihadConversation.new(determiner)
	local screenWidth = (g_window:getScreenSize())
	determiner = determiner or RegionDeterminer.new(screenWidth / 2)
	return setmetatable(base.new(determiner), XihadConversation)
end

function XihadConversation:_close_speaker_wait(speaker)
	base._close_speaker_wait(self, speaker)
	AsConditionFactory.waitTimer(0.5)
end

function XihadConversation:addWarrior(warriorName)
	local warrioDelegate = WarriorDelegate.fromObject(g_scene, c(warriorName))
	return self:addSpeaker(warrioDelegate)
end

return XihadConversation
