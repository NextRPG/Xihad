local Skill = {
	name  	= 'unknown',
	toEnemy = true,
	toLeague= false,
	toSelf  = false,
	
	range 	= nil,
	effect  = nil,
	animator= nil,
}
Skill.__index = Skill

function Skill.new(name, range, effect, animator, toEnemy, toLeague, toSelf)
	local obj = setmetatable({
			name  	= name,
			toSelf  = toSelf,
			toLeague= toLeague,
			toEnemy = toEnemy,
			
			range 	= range,
			effect  = effect,
			animator= animator,
		}, Skill)
	return obj
end

function Skill:isMultiTarget()
	return self.range:isMultiTarget()
end

function Skill:getName()
	return self.name
end

function Skill:getRange()
	return self.range
end

function Skill:canCastToSelf()
	return self.toSelf
end

function Skill:canCastToLeague()
	return self.toLeague
end

function Skill:canCastToEnemy()
	return self.toEnemy
end

return Skill
