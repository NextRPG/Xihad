local DefaultEntry = { }
DefaultEntry.__index = DefaultEntry

function DefaultEntry.new()
	local obj = setmetatable({ }, DefaultEntry)
	return obj
end

function SkillEntry:onHover()
	-- Do nothing
end

function SkillEntry:onSelected()
	
end

function SkillEntry:isEnabled()
	return true
end

function SkillEntry:getName()
	return skill:getName()
end

function SkillEntry:getValue()
	return nil
end

return DefaultEntry