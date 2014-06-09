local Array = require 'std.Array'
local functional = require 'std.functional'
local PromoteResult = {
	newSkills = nil,
	propertyIncs = nil,
}
PromoteResult.__index = PromoteResult

function PromoteResult.new()
	return setmetatable({
			newSkills = nil,
			propertyIncs = {},
		}, PromoteResult)
end

function PromoteResult:setNewSkills(list)
	self.newSkills = list
end

function PromoteResult:allNewSkills()
	if self.newSkills then
		return Array.elements(self.newSkills)
	else
		return functional.idle
	end
end

function PromoteResult:getPropertyInc(pname)
	return self.propertyIncs[pname]
end

function PromoteResult:setPropertyInc(pname, inc)
	self.propertyIncs[pname] = inc
end

return PromoteResult