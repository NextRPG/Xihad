local sCoroutine = require 'std.sCoroutine'
local ConcurrentJobs = {
	jobs = nil,
}
ConcurrentJobs.__index = ConcurrentJobs

function ConcurrentJobs.new()
	return setmetatable({
			jobs = {},
		}, ConcurrentJobs)
end

function ConcurrentJobs:addJob(f)
	table.insert(self.jobs, f)
end

function ConcurrentJobs:join()
	local current = coroutine.running()
	local runningJobs = #self.jobs
	
	for _, job in ipairs(self.jobs) do
		local co = coroutine.create(function ()
			job()
			
			runningJobs = runningJobs - 1
			if coroutine.status(current) == 'suspended' then
				sCoroutine.resume(current)
			end
		end)
		
		sCoroutine.resume(co)
	end
	
	while runningJobs > 0 do
		coroutine.yield()
	end
end

return ConcurrentJobs