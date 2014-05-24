local SkillRegistry = require 'Skill.SkillRegistry'
local CommandList = {
	source = nil,		-- warrior
	destination = nil,	-- location
	command = nil,		-- string?
	target  = nil,		-- location
}
CommandList.__index = CommandList

function CommandList:__tostring()
	if not self.source then
		return 'CommandList: no source warrior'
	end
	
	if not self.destination then
		return 'CommandList: no destination'
	end
	
	if not self.command then 
		return 'CommandList: no command'
	end
	
	local sourceName = self.source:getHostObject():getID()
	local prefix = sourceName..'到达'..tostring(self.destination)
	if not self.target then
		return prefix..self.command
	else
		return prefix..'朝向'..tostring(self.target)..'使用'..self.command
	end
end

function CommandList.new()
	return setmetatable({ }, CommandList)
end

function CommandList:setSource(warrior)
	self.source = warrior
end

function CommandList:getSource()
	return self.source
end

function CommandList:setLocation(location)
	self.destination = location:copy()
end

function CommandList:getLocation()
	return self.destination:copy()
end

function CommandList:setCommand(cmd)
	self.command = cmd
end

function CommandList:getCommand()
	return self.command
end

function CommandList:getAsCastable()
	local cmd = self.command
	if cmd == '待机' or cmd == '交换' then
		error('Can not getAsCastable')
	end
	
	local skill = SkillRegistry.findSkillByName(cmd)
	if skill then
		return skill
	end
	
	-- as item ?
end

function CommandList:setTarget(targetLocation)
	self.target = targetLocation:copy()
end

function CommandList:getTarget()
	return self.target
end

return CommandList
