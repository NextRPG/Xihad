local base = require 'Controller.PlayerState'
local SkillQuery = require 'Skill.SkillQuery'
local ChooseCommandState = setmetatable({
		commandRange = nil,
	}, base)
ChooseCommandState.__index = ChooseCommandState

function ChooseCommandState.new(...)
	return setmetatable(base.new(...), ChooseCommandState)
end

function ChooseCommandState:onStateEnter()
	self.ui:showCommandView(self:_getSource())
end

function ChooseCommandState:onStateExit()
	self:_safeClear('commandRange')
	self.ui:closeCommandView()
end

function ChooseCommandState:needTouch()
	return false
end

function ChooseCommandState:needHover()
	return false
end

function ChooseCommandState:onSelectCommand(command, subcommand)
	if command == '技能' then
		self.commandList:setCommand(command, subcommand)
		return 'next'
	elseif command == '交换' then
		return 'trade'
	end
	
	if command == '待机' then
		self.executor:standBy(self.commandList:getSource())
	elseif command == '道具' then
		self.executor:useItem(self:_getSource(), subcommand)
	end	
	
	if self:_getSource():isActive() then
		self.ui:updateCommandView(self:_getSource())
	else
		return 'done'
	end
end

function ChooseCommandState:_updateMark(subcommand)
	local range = SkillQuery.getLaunchableTiles(
					self:_getSource(), subcommand, true)
	
	if #range == 0 then
		io.err:write('Hover a non-launchable tile')
	else
		self:_markRange(range, 'Castable', 'commandRange')
	end
end

function ChooseCommandState:onHoverCommand(command, subcommand)
	self:_safeClear('commandRange')
	
	if command == '技能' then
		if subcommand ~= nil then
			self:_updateMark(subcommand)
		end
	end
end

function ChooseCommandState:onUICommand(command, subcommand, type)
	print(command, subcommand, type)
	if type == 'Select' then
		return self:onSelectCommand(command, subcommand)
	elseif type == 'Hover' then
		return self:onHoverCommand(command, subcommand)
	end
end

return ChooseCommandState
