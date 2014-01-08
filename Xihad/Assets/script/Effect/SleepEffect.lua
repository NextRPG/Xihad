require "BaseEffect"
SleepEffect = BaseEffect:new()


function SleepEffect:new( o )
	o = o or {}

	setmetatable(o, {__index = self})
	return o
end

function SleepEffect:bind( target )
	-- body
end

function SleepEffect:roundUpdate( target )
	if	self:checkUpdate( target ) then
		target:roundOver()
	end
	-- dispatch round over message
end

function SleepEffect:unbind( target )
	-- body
end
