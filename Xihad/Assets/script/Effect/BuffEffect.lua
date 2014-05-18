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

function BuffEffect:onBind(bindingWarrior)
	print('BuffEffect toAttach', self.field, self:getBinding():get(self.field))
	bindingWarrior:attachPropertyEffect(self.field, 'skill', self)
	print('BuffEffect attached', self.field, self:getBinding():get(self.field))
	
	-- TODO 添加动画
end

function BuffEffect:onUnbind()
	print('onUnbind BuffEffect')
	
	self:getBinding():detachPropertyEffect(self.field, 'skill', self)
	
	-- TODO 移除动画
	print('BuffEffect detached', self.field, self:getBinding():get(self.field))
end

function BuffEffect:getEquation()
	return self.equation
end

return BuffEffect