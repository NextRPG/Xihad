local Location = {
	x = 0,
	y = 0,
}
Location.__index = Location

function Location:__mul(n)
	local cpy = self:copy()
	cpy:mul(n)
	return cpy
end

function Location:__div(n)
	local cpy = self:copy()
	cpy:div(n)
	return cpy
end

function Location:__sub(b)
	local cpy = self:copy()
	cpy:sub(b)
	return cpy
end

function Location:__add(b)
	local cpy = self:copy()
	cpy:add(b)
	return cpy
end

function Location:__unm(b)
	local cpy = self:copy()
	copy:inv()
	return cpy
end

function Location:__eq(b)
	return self.x == b.x and self.y == b.y
end

function Location:__lt(b)
	if self.x < b.x then return true end
	if self.x > b.x then return false end
	return self.y < b.y
end

function Location:__le(b)
	return self.x <= b.x
end

function Location:__tostring()
	return 'Loc['..self.x..', '..self.y..']'
end

function Location.new(x, y)
	local o = setmetatable({}, Location)
	if type(x) == 'number' and type(y) == 'number' then
		o.x = x
		o.y = y
	end
	
	return o
end

function Location:copy()
	return Location.new(self.x, self.y)
end

function Location:left()
	return Location.new(self.x-1, self.y)
end

function Location:right()
	return Location.new(self.x+1, self.y)
end

function Location:top()
	return Location.new(self.x, self.y+1)
end

function Location:down()
	return Location.new(self.x, self.y-1)
end

function Location:xy()
	return self.x, self.y
end

function Location:sub(b)
	self.x = self.x - b.x
	self.y = self.y - b.y
end

function Location:add(b)
	self.x = self.x + b.x
	self.y = self.y + b.y
end

function Location:mul(n)
	self.x = self.x * n
	self.y = self.y * n
end

function Location:div(n)
	self.x = self.x / n
	self.y = self.y / n
end

function Location:inv()
	self.x = -self.x
	self.y = -self.y
end

function Location:set(x, y)
	if type(x) == 'number' then
		self.x = x
	end
	
	if type(y) == 'number' then
		self.y = y
	end
end

function Location:distance(loc)
	local dx = math.abs(self.x - loc.x)
	local dy = math.abs(self.y - loc.y)
	return dx+dy
end

function Location:eulerDistance(loc)
	local dx = self.x - loc.x
	local dy = self.y - loc.y
	return math.sqrt(dx*dx + dy*dy)
end

return Location
