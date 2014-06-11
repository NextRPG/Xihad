local Class = require 'std.Class'
local ModifierAdapter = require 'Modifier.ModifierAdapter'
local PlayerState = {
	ui = nil,
	camera = nil,
	painter = nil,
	executor = nil,
	commandList = nil,
	cursorFastener = nil,
}
PlayerState.__index = PlayerState

function PlayerState.new(sharedCommandList, 
		cursorFastener, ui, camera, painter, executor)
	assert(sharedCommandList ~= nil)
	return setmetatable({
			ui = ui,
			camera = camera,
			painter = painter, 
			executor = executor,
			commandList = sharedCommandList,
			cursorFastener = cursorFastener,
		}, PlayerState)
end

function PlayerState:_getSource()
	return self.commandList:getSource()
end

function PlayerState:_getSourceBarrier()
	local source = self:_getSource()
	if source then
		return source:findPeer(c'Barrier')
	end
end

function PlayerState:_getSourceTile()
	local barrier = self:_getSourceBarrier()
	if barrier then
		return barrier:getTile()
	end
end

function PlayerState:_getSourceObject()
	local source = self:_getSource()
	if source then
		return source:getHostObject()
	end
end

function PlayerState:_getSourceCaster()
	local source = self:_getSource()
	if source then
		return source:findPeer(c'SkillCaster')
	end
end

function PlayerState:_assignHandle(handle, field)
	if type(field) == 'string' then
		self[field] = handle
	else
		return handle
	end
end

function PlayerState:_markTile(tile, desc, field)
	local handle = self.painter:mark({ tile }, desc)
	return self:_assignHandle(handle, field)
end

function PlayerState:_markRange(range, desc, field)
	local handle = self.painter:mark(range, desc)
	return self:_assignHandle(handle, field)
end

function PlayerState:_clearMark(handle)
	self.painter:clear(handle)
end

function PlayerState:_safeClear(handleField)
	self.painter:clear(self[handleField])
	self[handleField] = nil
end

function PlayerState:_showTileInfo(tile)
	self.ui:showTileInfo(tile)
end

function PlayerState:_showWarriorInfo(warrior)
	self.ui:showWarriorInfo(warrior)
end

function PlayerState:_focusTile(tile)
	self.camera:focus(tile:getTerrain():getHostObject())
end

function PlayerState:_focusObject(object)
	self.camera:focus(object)
end

function PlayerState:_fastenCursorWhen(callback)
	local mod = self.cursorFastener:createFixCursorModifier()
	
	if not mod then
		callback()
	else
		local updater = ModifierAdapter.new(mod)
		g_scene:appendUpdater(updater)
		callback()
		updater:stop()
	end
end

function PlayerState:onStateEnter(state, prev)
end

function PlayerState:onStateExit(state, next)
end

function PlayerState:needTouch()
	return true
end

function PlayerState:needHover()
	return true
end

function PlayerState:needCDWhenTouch()
	return true
end

function PlayerState:needCDWhenHover()
	return true
end

function PlayerState:onBack()
	return 'back'
end

function PlayerState:onUICommand(command, subcommand, type)
end

function PlayerState:_onTile(tile, times, warriorFunc, vacancyFunc)
	local warrior = tile:getWarrior()
	if warrior then
		return self[warriorFunc](self, warrior:getHostObject(), times)
	else
		return self[vacancyFunc](self, tile, times)
	end
end

function PlayerState:_onWarrior(warriorObj, times, heroFunc, enemyFunc)
	if warriorObj:hasTag(c'Hero') then
		return self[heroFunc](self, warriorObj, times)
	else --if warriorObj:hasTag(c'Enemy') then
		return self[enemyFunc](self, warriorObj, times)
	-- else
	-- 	print('Non-hero and Non-AI person found')
	end
end

function PlayerState:onWarriorSelected(warriorObj, times)
	return self:_onWarrior(warriorObj, times, 
					'onHeroSelected', 'onEnemySelected')
end

function PlayerState:onTileSelected(tile, times)
	return self:_onTile(tile, times, 'onWarriorSelected', 'onVacancySelected')
end

function PlayerState:onVacancySelected(tileObject, times) 
end

function PlayerState:onHeroSelected(heroObject, times) 
end

function PlayerState:onEnemySelected(enemyObject, times) 
end

function PlayerState:onWarriorHovered(warriorObj)
	return self:_onWarrior(warriorObj, nil, 'onHeroHovered', 'onEnemyHovered')
end

function PlayerState:onTileHovered(tile)
	return self:_onTile(tile, nil, 'onWarriorHovered', 'onVacancyHovered')
end

function PlayerState:onVacancyHovered(tileObject) 
end

function PlayerState:onHeroHovered(heroObject) 
end

function PlayerState:onEnemyHovered(enemyObject) 
end

return PlayerState
