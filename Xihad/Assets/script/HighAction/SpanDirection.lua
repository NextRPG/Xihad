local SpanDirection = {
	
}
SpanDirection.__index = SpanDirection

function SpanDirection.new(dir1, dir2)
	local obj = setmetatable({ }, SpanDirection)
	return obj
end

return SpanDirection