local BaseSerializer = {}
BaseSerializer.__index = BaseSerializer

function BaseSerializer.new()
	return setmetatable({}, BaseSerializer)
end

function BaseSerializer:_serialize_primitive(write, o)
	if type(o) == 'string' then
		write(string.format('%q', o))
	elseif type(o) == 'number' then
		write(string.format('%d', o))
	elseif type(o) == 'boolean' then
		write(o and 'true' or 'false')
	elseif type(o) == 'nil' then
		write('nil')
	else
		error('can not serialize a ' .. type(o))
	end
end

function BaseSerializer:_serialize_table(write, o)
	error('no implementation by default')
end

function BaseSerializer:isValidKey(o)
	return type(o) == 'string'
		or type(o) == 'number'
		or type(o) == 'boolean'
end

function BaseSerializer:checkKey(o)
	if not self:isValidKey(o) then
		error('invalid key found in table: ' .. tostring(k))
	end
end

function BaseSerializer:serialize(write, o)
	if type(o) == 'table' then
		self:_serialize_table(write, o)
	else
		self:_serialize_primitive(write, o)
	end
end

return BaseSerializer