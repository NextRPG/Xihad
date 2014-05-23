local base = require 'Effect.BoundEffect'
local BuffEffect = setmetatable({
	field 	= nil,
	equation= nil,
}, base)

BuffEffect.__index = BuffEffect

function BuffEffect.new(field, equation, recycler)
	local obj = setmetatable(base.new(recycler), BuffEffect)
	obj.field = field
	obj.equation = equation
	
	return obj
end

function BuffEffect:_generateUID(cause)
	return cause .. '#' .. self.field
end

function BuffEffect:onBind(bindingWarrior)
	print('BuffEffect toAttach', self.field, self:getBinding():get(self.field))
	
	bindingWarrior:addProperty(self.field, self.equation)
	
	print('BuffEffect attached', self.field, self:getBinding():get(self.field))
end

function BuffEffect:onUnbind(bindingWarrior)
	print('onUnbind BuffEffect')
	
	bindingWarrior:addProperty(self.field, -self.equation)
	
	print('BuffEffect detached', self.field, self:getBinding():get(self.field))
end

return BuffEffect