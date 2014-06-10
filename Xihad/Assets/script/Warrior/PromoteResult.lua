local Array = require 'std.Array'
local Iterator = require 'std.Iterator'
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

function PromoteResult:_apply_skill_list(warrior)
	local caster = warrior:findPeer(c'SkillCaster')
	for skill, count in self:allNewSkills() do
		caster:learnSkill(skill, count)
	end
end

function PromoteResult:_apply_property_incs(warrior)
	for pname, inc in pairs(self.propertyIncs) do
		warrior:addBasic(pname, inc)
	end
end

function PromoteResult:apply(warrior)
	self:_apply_skill_list(warrior)
	self:_apply_property_incs(warrior)
end

function PromoteResult:setNewSkills(list)
	self.newSkills = list
end

function PromoteResult:allNewSkills()
	if self.newSkills then
		return Iterator.transform(
			function(skillPair)
				if skillPair then
					return skillPair.skill, skillPair.count
				end
			end,
			Array.elements(self.newSkills))
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

function PromoteResult:__tostring()
	local ret = 'Learned skills:\n'
	for skill, count in self:allNewSkills() do
		ret = string.format('%s\t%s: %d\n', ret, skill:getName(), count)
	end
	
	ret = ret .. 'PropertyIncs:\n'
	for pname, inc in pairs(self.propertyIncs) do
		ret = string.format('%s\t%s: %d\n', ret, pname, inc)
	end
	
	return ret
end

return PromoteResult