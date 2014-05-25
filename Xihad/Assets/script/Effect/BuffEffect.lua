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
	return string.format('%q, %q', cause, self.field)
end

local function log(...) 
	print(...)
end

function BuffEffect:onBind(bindingWarrior)
	log('BuffEffect toAttach', self.field, self:getBinding():get(self.field))
	
	bindingWarrior:addProperty(self.field, self.equation)
	
	log('BuffEffect attached', self.field, self:getBinding():get(self.field))
end

function BuffEffect:onUnbind(bindingWarrior)
	log('onUnbind BuffEffect')
	
	bindingWarrior:addProperty(self.field, -self.equation)
	
	log('BuffEffect detached', self.field, self:getBinding():get(self.field))
end

return BuffEffect