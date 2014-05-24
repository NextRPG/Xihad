local BaseItem = require 'Item.BaseItem'
local SimpleItemFactory = {}

function SimpleItemFactory.recovery(name, icon, desc, maxOverlay, recovery)
	local item = BaseItem.new(name, icon, desc, maxOverlay)
	function item:_onUsed(warrior)
		warrior:takeRecovery(recovery)
	end
	
	return item
end

function SimpleItemFactory.skillBook(name, icon, desc, skillName, initCount)
	local item = BaseItem.new(name, icon, desc, 1)
	local skill= SkillRegistry.findSkillByName(skillName)
	function item:_onUsed(warrior)
		warrior:learnSkill(skill, initCount)
	end
	
	return item
end

function SimpleItemFactory.pitfall()
	-- TODO add pitfall barrier onto chessboard
end

return SimpleItemFactory
