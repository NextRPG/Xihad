local Class = require 'std.Class'
local functional = require 'std.functional'
local ReadonlyWarrior = {}

function ReadonlyWarrior.from(warrior)
	local wrapped = Class.copy(warrior)
	
	wrapped.takeDamage 	= functional.idle
	wrapped.takeRecovery= functional.idle
	wrapped.activate 	= functional.idle
	wrapped.deactivate 	= functional.idle
	wrapped.addProperty = functional.idle
	wrapped.addBasic 	= functional.idle
	
	wrapped.addRoundListener 	= functional.idle
	wrapped.removeRoundListener = functional.idle
	wrapped.addPropertyListener = functional.idle
	wrapped.removePropertyListener 	= functional.idle
	
	wrapped.registerStickyEffect 	= functional.idle
	wrapped.unregisterStickyEffect 	= functional.idle
	wrapped.registerExclusiveEffect = functional.idle
	wrapped.unregisterExclusiveEffect = functional.idle
	
	return wrapped
end

return ReadonlyWarrior
