local Parcel = {
	items = nil,	-- { Item -> count }
}
Parcel.__index = Parcel

function Parcel.new()
	local o  = setmetatable({
			items = {},
		}, Parcel)
	
	return o
end

function Parcel:gainItem(item, count)
	
end

function Parcel:useItem(item)
	
end

function Parcel:canUse(item)
	
end

function Parcel:allItems()
	
end

function Parcel:hasItem()
	return #self.items >= 1
end

return Parcel