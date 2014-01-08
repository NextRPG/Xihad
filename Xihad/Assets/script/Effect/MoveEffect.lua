require "BaseEffect"

MoveEffect = BaseEffect:new{
	destination,
	interval,
	enable = false
}

function MoveEffect:new( o )
	o = o or {}
	setmetatable(o, {__index = self})


	return o
end

function MoveEffect:bind( target )
	for k,equation in pairs(target.properties) do
		equation:addPower(self.buffs[k].power)
		equation:addFixed(self.buffs[k].fixed)
	end
end

function MoveEffect:roundUpdate( target )
	self:checkUpdate( target )
end

function MoveEffect:unbind( target )
	for k,equation in pairs(target.properties) do
		equation:addPower(self.buffs[k].power)
		equation:addFixed(self.buffs[k].fixed)
	end
end