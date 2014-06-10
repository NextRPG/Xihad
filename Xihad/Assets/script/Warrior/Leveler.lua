local Leveler = {
	exp 	= 0,
	level 	= 1,
	grader 	= nil,
}
Leveler.__index = Leveler

function Leveler.new( data, object )
	assert(data.grader ~= nil)
	local obj = setmetatable({
			exp    = data.exp,
			level  = data.level,
			grader = data.grader,
		}, Leveler)
	return obj
end

function Leveler:getLevel()
	return self.level
end

function Leveler:getRestExpToNext()
	return self:getNextLevelExp() - self:getCurrentExp()
end

function Leveler:getCurrentExp()
	return self.exp
end

function Leveler:getNextLevelExp()
	return self.grader:getNextLevelExp(self.level)
end

function Leveler:_reset_to_next()
	self.exp = 0
	self.level = self.level + 1
end

function Leveler:_do_promote()
	return self.grader:promote(self:findPeer(c'Warrior'), self.level + 1)
end

function Leveler:_to_next_level(usedExp, callback)
	callback(usedExp, self:_do_promote())
	self:_reset_to_next()
end

function Leveler:_obtainExp(exp, callback)
	local nextExp = self.exp + exp
	while true do
		local full = self:getNextLevelExp()
		assert(full > self.exp)
		
		if nextExp < full then break end
		
		self:_to_next_level(full - self.exp, callback)
		nextExp = nextExp - full
	end
	
	callback(nextExp - self.exp)
	self.exp = nextExp
end

function Leveler:obtainExp(exp, callback)
	assert(exp >= 0)
	if exp > 0 then 
		self:_obtainExp(exp, callback)
	end
end

return Leveler