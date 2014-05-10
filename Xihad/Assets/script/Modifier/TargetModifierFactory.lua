local TargetModifier = require 'Modifier.TargetModifier'
local TargetModifierFactory = {}

function TargetModifierFactory.number(speed, target, variable)
	local action = TargetModifier.new(speed, target, variable)
	
	function action.setLength(self, delta, length) 
		assert(delta ~= 0)
		return delta / math.abs(delta) * length
	end
	
	function action.between(self, current, expect, target)
		return (expect - current) * (expect - target) < 0
	end
	
	return action
end

function TargetModifierFactory.vector(speed, target, variable)
	local action = TargetModifier.new(speed, target, variable)
	
	function action.setLength(self, delta, length)
		delta:setLength(length)
		return delta
	end
	
	function action.between(self, current, expect, target)
		return (expect - current):dot(expect - target) < 0
	end
	
	return action
end

return TargetModifierFactory
