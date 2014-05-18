local base = require 'Effect.BoundEffect'
local BuffEffect = setmetatable({
	field 	= nil,
	equation= nil,
}, base)

BuffEffect.__index = BuffEffect

function BuffEffect.new(field, equation, round)
	local obj = setmetatable(base.new(round), BuffEffect)
	obj.field = field
	obj.equation = equation
	
	return obj
end

function BuffEffect:onBind(bindingWarrior)
	print('BuffEffect toAttach', self.field, self:getBinding():get(self.field))
	bindingWarrior:attachState(self.field, 'skill', self)
	print('BuffEffect attached', self.field, self:getBinding():get(self.field))
	
	-- TODO 添加动画
end

function BuffEffect:onUnbind()
	self:getBinding():detachState(self.field, 'skill', self)
end

function BuffEffect:onAttached()
	assert(self:isBound())
end

---
-- 可能是由于unbind操作而主动脱离；
-- 也可能是由于其他技能影响了该属性，而被动脱离。
function BuffEffect:onDetached()
	if self:isBound() then
		self:unbind()
	end
	
	print('BuffEffect detached', self.field, self:getBinding():get(self.field))
	-- TODO 移除动画
end

function BuffEffect:getEquation()
	return self.equation
end

return BuffEffect