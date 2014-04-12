local Location = {
	x = 0,
	y = 0,
}
Location.__index = Location

Location.__sub = function (a, b)
	local cpy = a:copy()
	cpy:sub(b)
	return cpy
end

Location.__add = function (a, b)
	local cpy = a:copy()
	cpy:add(b)
	return cpy
end

Location.__unm = function (a, b)
	local cpy = a:copy()
	copy:inv()
	return cpy
end

Location.__eq = function (a, b)
	return a.x == b.x and a.y == b.y
end

Location.__lt = function (a, b)
	if a.x < b.x then return true end
	if a.x > b.x then return false end
	return a.y < b.y
end

Location.__le = function (a, b)
	return a.x <= b.x
end

function Location.new(x, y)
	local o = {}
	setmetatable(o, Location)
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
