require "Effect.BaseEffect"

BuffEffect = BaseEffect:new()

package.path = package.path .. ";../?.lua"
require "Consts"

function BuffEffect:new( o )
	o = o or {}
	setmetatable(o, {__index = self})

	o.buffs = o.buffs or {}

	return o
end

function BuffEffect:bind( target )
	for k,equation in pairs(target.properties) do
		equation:addPower(self.buffs[k].power)
		equation:addFixed(self.buffs[k].fixed)
	end
end

function BuffEffect:roundUpdate( target )
	self:checkUpdate( target )
end

function BuffEffect:unbind( target )
	for k,equation in pairs(target.properties) do
		equation:addPower(self.buffs[k].power)
		equation:addFixed(self.buffs[k].fixed)
	end
end
