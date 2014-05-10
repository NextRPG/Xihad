local ContinuousMouse = {}
ContinuousMouse.__index = ContinuousMouse

function ContinuousMouse.new()
	local obj = setmetatable({}, ContinuousMouse)
	return obj
end

function ContinuousMouse:update(time)
	
end

return ContinuousMouse