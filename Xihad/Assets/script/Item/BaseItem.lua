local BaseItem = {
	_name = nil,
	_icon = nil,
	_desc = nil,
	_maxOverlay = 99,
	_occupyRound= true,
	
	usages = { 
		default = '@warrior使用了@item',
	},
}
BaseItem.__index = BaseItem
local usageMT = { __index = BaseItem.usages }

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
			
			usages = setmetatable({}, usageMT),
		}, BaseItem)
end

function BaseItem:occupyRound()
	return self._occupyRound
end

function BaseItem:getMaxOverlay()
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

function BaseItem:canUse(warrior)
	return true
end

function BaseItem:_parseUsage(warrior, raw)
	local u1 = string.gsub(raw, '@warrior', warrior:getName())
	return string.gsub(u1, '@item', self._name)
end

---
-- @return used count
function BaseItem:onUsed(warrior)
	local usageId, loss = self:_onUsed(warrior)
	
	local usage = self.usages[usageId  or 'default']
	
	if loss == nil then 
		loss = true
	end
	
	return self:_parseUsage(warrior, usage), loss
end

return BaseItem