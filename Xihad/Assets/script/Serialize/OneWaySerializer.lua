local base = require 'Serialize.BaseSerializer'
local OneWaySerializer = setmetatable({}, base)
OneWaySerializer.__index = OneWaySerializer

function OneWaySerializer.new()
	return setmetatable(base.new(), OneWaySerializer)
end

function OneWaySerializer:_serialize_table(write, o)
	write('{\n')
	for k, v in pairs(o) do
		write('\t[')
		self:checkKey(k)
		self:serialize(write, k)
		write('] = ')
		self:serialize(write, v)
		write(',\n')
	end
	write('}')
end

if select('#', ...) == 0 then 
	local o = {
		'first',
		'second',
		a = 1,
		b = true,
		c = nil,
		d = 'abc',
		['quote " in me'] = {
			c = 2
		},
	}
	
	local serializer = OneWaySerializer.new()
	serializer:serialize(io.write, o)
end

return OneWaySerializer