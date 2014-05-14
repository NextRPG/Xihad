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

function ChooseTargetState:_getWC()
	return 	self:_getSource(), self.commandList:getCommand()
			
end

function ChooseTargetState:_getLaunchableTiles()
	local warrior, command = self:_getWC()
	local skill = SkillRegistry.findSkillByName(command)
	return skill:getLaunchableTiles(g_chessboard, warrior, warrior:getLocation())
end

function ChooseTargetState:onStateEnter()
	assert(not self.castableHandle)
	local tiles = self:_getLaunchableTiles()
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
	local tiles = self:_getLaunchableTiles()
	if not tiles[tile] then
		print('not castable', tostring(tile:getLocation()))
	else
		self.commandList:setTarget(tile:getLocation())
		local source, cmd = self:_getWC()
		self.executor:cast(source, self.commandList:getTarget(), cmd)
		return 'next'
	end
end

function ChooseTargetState:onTileHovered(tile)
	-- show all attackable enemies' status
	-- 估算所有敌人受伤害后的血量值
	
end

return ChooseTargetState
