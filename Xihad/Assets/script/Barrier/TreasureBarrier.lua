local base = require 'Barrier.OnceBarrier'
local TreasureBarrier = setmetatable({
		item = item,
		count= 1,
	}, base)
TreasureBarrier.__base = 'Barrier'
TreasureBarrier.__index= TreasureBarrier

function TreasureBarrier.new(item, count)
	return setmetatable(base.new('打开'), TreasureBarrier)
end

function TreasureBarrier:_onSurveyed(warrior, times)
	local parcel = warrior:findPeer(c'Parcel')
	parcel:gainItem(self.item, self.count)
	return true
end

return TreasureBarrier