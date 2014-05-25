local ItemRegistry = {
	data = {}
}

function ItemRegistry.findItemByName(itemName)
	return ItemRegistry.data[itemName]
end

local function addItem(item)
	ItemRegistry.data[item:getName()] = item
end

ItemRegistry.addItem = addItem


local Equation = require 'Warrior.Equation'
local BuffEffect = require 'Effect.BuffEffect'
local EquipmentItem = require 'Item.EquipmentItem'
local SimpleItemFactory = require 'Item.SimpleItemFactory'
addItem(SimpleItemFactory.recovery('伤药', nil, '治疗百病', 9, 20))
addItem(EquipmentItem.new('长矛', nil, '很长的矛', '武器', BuffEffect.new('ATK', Equation.new(10))))
return ItemRegistry
