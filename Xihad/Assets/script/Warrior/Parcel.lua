local Parcel = {
	items = nil,	-- { Item -> count }
}

function Parcel.new()
	local o  = setmetatable({
			items = {},
		}, Parcel)
	
	return o
end

function Parcel:gainItem(item)
	
end

function Parcel:useItem(item)
	
end

return Parcel