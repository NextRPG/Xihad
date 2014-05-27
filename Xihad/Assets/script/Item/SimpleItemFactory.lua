local BaseItem = require 'Item.BaseItem'
local SimpleItemFactory = {}

function SimpleItemFactory.recoverSkill(name, icon, desc, maxOverlay, percentage)
	local item = BaseItem.new(name, icon, desc, maxOverlay)
	item.usages.default = '@warrior服用了@item，瞬间生龙活虎'
	
	function item:_onUsed(warrior)
		warrior:findPeer(c'SkillCaster'):recovery(percentage)
	end
end

function SimpleItemFactory.recovery(name, icon, desc, maxOverlay, recovery)
	local item = BaseItem.new(name, icon, desc, maxOverlay)
	item.usages.default = '@warrior服用了@item，瞬间精力充沛'
	
	function item:canUse(warrior)
		return warrior:isInjured()
	end
	
	function item:_onUsed(warrior)
		warrior:takeRecovery(recovery)
	end
	
	return item
end

function SimpleItemFactory.skillBook(name, icon, desc, skillName, initCount)
	local item = BaseItem.new(name, icon, desc, 1)
	item.usages.default = '@warrior一番研读之后，学会了新技能：'..skillName
	
	local skill= SkillRegistry.findSkillByName(skillName)
	function item:canUse(warrior)
		return not warrior:findPeer(c'SkillCaster'):hasLearnedSkill(skill)
	end
	
	function item:_onUsed(warrior)
		warrior:findPeer(c'SkillCaster'):learnSkill(skill, initCount)
	end
	
	return item
end

function SimpleItemFactory.pitfall()
	-- TODO add pitfall barrier onto chessboard
end

return SimpleItemFactory
