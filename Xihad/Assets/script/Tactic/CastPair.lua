local CastPair = {
	skill = nil,
	targetLoc = nil,
}
CastPair.__index = CastPair

function CastPair.new(skill, target)
	return setmetatable({
			skill = skill,
			targetLoc = target:copy(),
		}, CastPair)
end

function CastPair:decode()
	return self.skill, self.targetLoc:copy()
end

function CastPair:__eq(other)
	return self.skill:getName() == other.skill:getName()
		and self.targetLoc	== other.targetLoc
end

function CastPair:__lt(other)
	if self.skill:getName() < other.skill:getName() then
		return true
	elseif self.skill:getName() > other.skill:getName() then
		return false
	end
	
	return self.targetLoc < other.targetLoc
end

return CastPair