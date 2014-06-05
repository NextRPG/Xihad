local RegionDeterminer = {
	centerSeparator = nil,
	padingX = nil,
	padingY = nil,
	relativeX = nil,
}
RegionDeterminer.__index = RegionDeterminer

function RegionDeterminer.new(centerSeparator, padingX, padingY, relativeX)
	assert(type(centerSeparator) == 'number')
	return setmetatable({
			centerSeparator = centerSeparator,
			padingX = math.abs(padingX or 10),
			padingY = math.abs(padingY or 10),
			relativeX = math.abs(relativeX or 0.1),
		}, RegionDeterminer)
end

function RegionDeterminer:_set_position(speaker, onLeft)
	local sign = onLeft and 1 or -1
	speaker:setDialoguePosition(self.padingX * sign, -self.padingY)
	speaker:setIconRelativeX(self.relativeX * sign)
end

function RegionDeterminer:updatePosition(speaker, speakerPos, rest)
	local onLeft
	if rest:getCount() ~= 1 then
		onLeft = speakerPos.x < self.centerSeparator
	else
		onLeft = speakerPos.x < rest:getPosition(1).x
	end
	
	-- onLeft = true
	self:_set_position(speaker, onLeft)
	return onLeft and 'left' or 'right'
end

return RegionDeterminer