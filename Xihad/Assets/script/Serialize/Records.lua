local Records = {
	serializer = nil,
	allRecords = nil,
}
Records.__index = Records

function Records.new(serializer)
	return setmetatable({
			serializer = serializer,
			allRecords = {},
		}, Records)
end

function Records:_check_key(key)
	self.serializer:checkKey(key)
	if self.allRecords[key] ~= nil then
		error('duplicated record key: ' .. tostring(key))
	end
end

function Records:_set_key_value(k, v)
	self.allRecords[k] = v
end

function Records:newRecords(key)
	return self:addRecord(key, Records.new(nil))
end

function Records:addRecord(key, record)
	self:_check_key(key)
	self:_set_key_value(key, record:createSerializable())
	return record
end

function Records:addValue(key, o)
	self:_check_key(key)
	self:_set_key_value(key, o)
	return o
end

function Records:createSerializable()
	return self.allRecords
end

function Records:serailize(write)
	write('return\n')
	self.serializer:serailize(write, self.allRecords)
end

return Records
