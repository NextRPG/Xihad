--- 
-- Equation is constant value, non-changable
local Equation = {
	_power = 0,
	_offset= 0,
	_fixed = 0,
}
Equation.__index = Equation

function Equation.new(defaultOffset, power, fixed)
	return setmetatable({
			_offset= defaultOffset,
			_power = power,
			_fixed = fixed,
		}, Equation)
end

function Equation:__unm()
	local ret = Equation.new()
	ret._power = -self._power
	ret._offset= -self._offset
	ret._fixed = -self._fixed
	return ret
end

function Equation:__add(other)
	local ret = Equation.new()
	ret._power = self._power + other._power
	ret._offset= self._offset+ other._offset
	ret._fixed = self._fixed + other._fixed
	return ret
end 

function Equation:__sub(other)
	local ret = Equation.new()
	ret._power = self._power - other._power
	ret._offset= self._offset- other._offset
	ret._fixed = self._fixed - other._fixed
	return ret
end

function Equation:calculate( )
	return (self._power + 1) * self._offset + self._fixed
end

return Equation