local Warrior = require 'Warrior'

-- register game specific properties
Warrior.registerProperty('MHP')
Warrior.registerProperty('ATK')
Warrior.registerProperty('DFS')
Warrior.registerProperty('MTK')
Warrior.registerProperty('MDF')
Warrior.registerProperty('MAP')

local originalStart = Warrior.onStart
if originalStart ~= nil then 
	function Warrior:onStart()
		self:_setHitPoint(self:getMHP())
		originalStart(self)
	end
else
	function Warrior:onStart()
		self:_setHitPoint(self:getMHP())
	end
end

return Warrior
