require "BaseEffect"
PoisonEffect = BaseEffect.new()

function PoisonEffect:new( o )
	o = o or {}
	setmetatable(o, {__index = self})

	o.damage = o.damage or 0
	return o
end

function PoisonEffect:bind( target )
	assert(target, "target can't be nil")
	target.effects:add(self)
end

function PoisonEffect:roundUpdate( target )
	if self:checkUpdate(target) then
		target:handleDamage(self.damage)
	end
	-- dispatch poison message
end

function PoisonEffect:unbind( target )
	-- body
end





