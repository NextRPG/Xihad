local base = require 'Controller.PlayerState'
local Array= require 'std.Array'
local SkillRegistry = require 'Skill.SkillRegistry'

local ChooseTargetState = setmetatable({
	castableHandle = nil,
}, base)
ChooseTargetState.__index = ChooseTargetState

function ChooseTargetState.new(...)
	return setmetatable(base.new(...), ChooseTargetState)
end

function ChooseTargetState:onStateEnter()
	assert(not self.castableHandle)
	local command = self.commandList:getCommand()
	local skill = SkillRegistry.findSkillByName(command)
	
	local warrior = self.commandList:getSource()
	local location = self.commandList:getLocation()
	local tiles = skill:getLaunchableTiles(g_chessboard, warrior, location)
	
	self.castableHandle = self.painter:mark(Array.keys(tiles), 'Castable')
end

function ChooseTargetState:onStateExit()
	assert(self.castableHandle)
	self:_safeClear('castableHandle')
end

function ChooseTargetState:needCDWhenTouch()
	return false
end

function ChooseTargetState:needCDWhenHover()
	return false
end

function ChooseTargetState:onTileSelected(tile)
	-- self.executor:cast()
	-- attack
	-- TODO CHECK
	self.commandList:setTarget(tile:getLocation())
	
	-- self.camera:focus(nil)
end

function ChooseTargetState:onTileHovered(tile)
	-- show all attackable enemies' status
	-- 估算所有敌人受伤害后的血量值
	
end

return ChooseTargetState
