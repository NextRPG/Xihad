local ItemEntry = {
	parcel = nil,
	item = nil,
}
ItemEntry.__index = ItemEntry

function ItemEntry.new()
	local obj = setmetatable({ }, ItemEntry)
	return obj
end

function ItemEntry:onHover()
	
end

function ItemEntry:onSelected()
	local itemType = item:getType()
	if itemType == 'equipment' then
		-- equip/unequip it directly
	elseif itemType == 'castable' then
		-- the same as skill
	elseif itemType == ''
end

function ItemEntry:isEnabled()
	return skillCaster:canCast(skill)
end

function ItemEntry:getName()
	return skill:getName()
end

function ItemEntry:getValue()
	return skillCaster:getRestCount(skill)
end

return ItemEntry