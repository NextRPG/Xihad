local base = require 'Controller.PlayerState'
local SkillQuery = require 'Skill.SkillQuery'

local ChooseTargetState = setmetatable({
	castableHandle = nil,
}, base)
ChooseTargetState.__index = ChooseTargetState

function ChooseTargetState.new(...)
	return setmetatable(base.new(...), ChooseTargetState)
end

function ChooseTargetState:_getWC()
	local command, subcommand = self.commandList:getCommand()
	assert(command == '技能', command)
	return 	self:_getSource(), subcommand
			
end

function ChooseTargetState:_getLaunchableTiles(array)
	local w, c = self:_getWC()
	return SkillQuery.getLaunchableTiles(w, c, array)
end

function ChooseTargetState:onStateEnter()
	assert(not self.castableHandle)
	local tiles = self:_getLaunchableTiles(true)
	self:_markRange(tiles, 'Castable', 'castableHandle')
end

function ChooseTargetState:onStateExit()
	self:_safeClear('castableHandle')
end

function ChooseTargetState:needCDWhenTouch()
	return false
end

function ChooseTargetState:needCDWhenHover()
	return false
end

function ChooseTargetState:onTileSelected(tile)
	local tiles = self:_getLaunchableTiles(false)
	if not tiles[tile] then
		print('not castable', tostring(tile:getLocation()))
	else
		self:_safeClear('castableHandle')
		
		self.commandList:setTarget(tile:getLocation())
		local source, skill = self:_getWC()
		self.executor:cast(source, self.commandList:getTarget(), skill)
		return 'next'
	end
end

function ChooseTargetState:onTileHovered(tile)
	-- local source, command = self:_getWC()
	-- local caster = source:findPeer(c'SkillCaster')
	-- local skill = SkillRegistry.findSkillByName(command)
	-- local result = caster:castSkill(skill, tile:getLocation(), g_chessboard)
end

return ChooseTargetState
