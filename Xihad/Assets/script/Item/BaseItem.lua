local BaseItem = {
	_name = nil,
	_icon = nil,
	_desc = nil,
	_maxOverlay = 99,
	_occupyRound= true,
}
BaseItem.__index = BaseItem

---
-- 装备、技能书、陷阱
-- 如果需要占用回合，则玩家确认后onUsed()；
-- 	否则，直接onUsed()；
function BaseItem.new(name, icon, desc, maxOverlay, occupyRound)
	return setmetatable({
			_name = name,
			_icon = icon,
			_desc = desc,
			_maxOverlay = maxOverlay,
			_occupyRound= occupyRound,
		}, BaseItem)
end

function BaseItem:_occupyRound()
	return self._occupyRound
end

function BaseItem:getMaxOverlayCount()
	return self._maxOverlay
end

function BaseItem:getName()
	return self._name
end

function BaseItem:getIcon()
	return self._icon
end

function BaseItem:getDescription()
	return self._desc
end

---
-- Invoked when warrior discard this item or exchange to other
function BaseItem:onDiscarded(warrior, count)
	
end

function BaseItem:_onUsed(warrior)
	error('No implementation by default')
end

---
-- @return used count
function BaseItem:onUsed(warrior)
	self:_onUsed(warrior)
	return 1
end

return BaseItem