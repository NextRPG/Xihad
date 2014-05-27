local SkillRegistry = require 'Skill.SkillRegistry'
local CommandList = {
	source = nil,		-- warrior
	destination = nil,	-- location
	mainCommand = nil,	-- string
	subCommand  = nil,	-- string
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
	
	if not self.mainCommand then 
		return 'CommandList: no command'
	end
	
	local sourceName = self.source:getHostObject():getID()
	local prefix = sourceName..'到达'..tostring(self.destination)
	if not self.subCommand then
		return prefix..self.mainCommand
	elseif self.target then
		prefix = prefix..'朝向'..tostring(self.target)
	end
	
	return prefix..'使用'..self.subCommand
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

function CommandList:setCommand(mainCommand, subCommand)
	self.mainCommand = mainCommand
	self.subCommand  = subCommand
end

function CommandList:getCommand()
	return self.mainCommand, self.subCommand
end

function CommandList:checkSkill()
	if self.mainCommand ~= '技能' then
		error('Can not checkSkill')
	end
	
	return SkillRegistry.findSkillByName(self.subCommand)
end

function CommandList:checkItem()
	
end

function CommandList:setTarget(targetLocation)
	self.target = targetLocation:copy()
end

function CommandList:getTarget()
	return self.target
end

return CommandList
