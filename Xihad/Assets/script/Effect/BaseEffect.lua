BaseEffect = {
	rounds = 0,
	enableUpdate = false
}

function BaseEffect:new( o )
	o = o or {}
	setmetatable(o, {__index = self})

	return o
end

function BaseEffect:checkUpdate( target )
	if self.enableUpdate == false then
		return false
	elseif (self.rounds == 0) then
		target:unbindEffect(self)
		return false
	end
	self.rounds = self.rounds - 1;
	return true
end