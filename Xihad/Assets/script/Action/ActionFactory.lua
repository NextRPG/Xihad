local seq = require 'Action.SequenceAction'
local pal = require 'Action.ParallelAction'	

local factory = {}

function factory.sequence(array)
	return seq.new(array)
end

function factory.parallel(array)
	return pal.new(array)
end

function factory.loop(t, n)
	local l = {}
	local looped = 0
	function l.onUpdate(self, time)
		t:update(time)
		if t:hasFinished() then
			t:reset()
			looped = looped+1
		end
	end
	
	function l.reset(self)
		t:reset()
		looped = 0
	end
	
	function l:hasFinished()
		return looped >= n
	end
end

return factory
