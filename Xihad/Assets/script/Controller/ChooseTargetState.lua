local base = require 'Controller.PlayerState'
local SkillQuery = require 'Skill.SkillQuery'
local SkillRegistry = require 'Skill.SkillRegistry'

local ChooseTargetState = setmetatable({
	hoveringTile = nil,
	impactHanlde = nil,
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

function ChooseTargetState:_getSkill()
	local command, subcommand = self.commandList:getCommand()
	assert(command == '技能', command)
	return SkillRegistry.findSkillByName(subcommand)
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

function ChooseTargetState:_clearHandle()
	self:_safeClear('castableHandle')
	self:_safeClear('impactHanlde')
end

function ChooseTargetState:onStateExit()
	self:_clearHandle()
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
		self:_clearHandle()
		self.commandList:setTarget(tile:getLocation())
		local source, skill = self:_getWC()
		self.executor:cast(source, self.commandList:getTarget(), skill)
		return 'next'
	end
end

function ChooseTargetState:_showImpactTiles()
	local skill = self:_getSkill()
	local center= self.hoveringTile:getLocation()
	local range = skill:getImpactTiles(g_chessboard, center)
	self:_markRange(range, 'Impact', 'impactHanlde')
end

function ChooseTargetState:_showHitPointEstimate()
	
end

function ChooseTargetState:_updateHover()
	self:_safeClear('impactHanlde')
	
	if self.hoveringTile then
		self:_showImpactTiles()
	end
end

function ChooseTargetState:onTileHovered(tile)
	if not self:_getLaunchableTiles()[tile] then
		tile = nil
	end
	
	if self.hoveringTile ~= tile then
		self.hoveringTile = tile
		self:_updateHover()
	end
end

return ChooseTargetState
