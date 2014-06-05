local Class = require 'std.Class'
local WarriorDelegate = {
	warrior = nil,
}
WarriorDelegate.__index = WarriorDelegate

function WarriorDelegate.new(warrior)
	assert(warrior ~= nil)
	return setmetatable({
			warrior = warrior,
		}, WarriorDelegate)
end

function WarriorDelegate.fromObject(scene, name)
	local object = scene:findObject(name)
	return WarriorDelegate.new(object:findComponent(c'Warrior'))
end

function WarriorDelegate:getWorldPosition()
	return self.warrior:getHostObject():getTranslate()
end

Class.delegate(WarriorDelegate, 'getName', 'warrior')

function WarriorDelegate:getIcon()
	-- TODO
	return 'Character/bbbb_sad'
end

return WarriorDelegate