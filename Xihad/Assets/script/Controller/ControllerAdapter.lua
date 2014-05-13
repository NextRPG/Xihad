local Class = require 'std.Class'
local ControllerAdapter = {}

function ControllerAdapter.new(impl)
	local o = {}
	
	if impl.onKeyUp then
		Class.delegateClosure(o, 'onKeyUp', impl)
	end
	
	if impl.onKeyDown then
		Class.delegateClosure(o, 'onKeyDown', impl)
	end
	
	if impl.onMouseEvent then
		Class.delegateClosure(o, 'onMouseEvent', impl)
	end
	
	return o
end

return ControllerAdapter