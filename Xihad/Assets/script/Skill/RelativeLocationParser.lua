local RelativeLocationParser = {
	includeCenter = '@',
	excludeCenter = '=',
	includeLocation = 'o',
	newLine = '\n'
}
RelativeLocationParser.__index = RelativeLocationParser

function RelativeLocationParser.new()
	return setmetatable({}, RelativeLocationParser)
end

function RelativeLocationParser:_parse(range)
	local width = string.find(range, self.newLine) or #range + 1
	assert(width > 1)
	
	local centerPattern= string.format('[%s%s]', self.includeCenter, self.excludeCenter)
	local centerIndex  = assert(string.find(range, centerPattern))
	assert(not string.find(range, centerPattern, centerIndex+1))
	
	if range:sub(centerIndex, centerIndex) == self.includeCenter then
		coroutine.yield(0, 0)
	end
	
	local centerX, centerY = centerIndex%width, math.ceil(centerIndex/width)
	local x, y = 1, 1
	for i = 1, #range do
		local ch = range:sub(i, i)
		
		if x == width then
			assert(ch == self.newLine)
		elseif ch == self.includeLocation then
			coroutine.yield(x - centerX, centerY - y)
		end
		
		x = x + 1
		if x > width then
			x = 1
			y = y + 1
		end
	end
end

function RelativeLocationParser:parse(range)
	return coroutine.wrap(function ()
		self:_parse(range)
	end)
end

if select('#', ...) == 0 then 
	local parser = RelativeLocationParser.new()
	for x, y in parser:parse('oxx=oxx') do
		print(x, y)
	end
end

return RelativeLocationParser