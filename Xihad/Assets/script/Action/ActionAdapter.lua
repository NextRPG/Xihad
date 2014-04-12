local adapter = {}

function adapter.new(action)
	local a = {}
	function a.onUpdate(self)
		action:update(g_time.change)
	
		if action:hasFinished() then
			self:stop()
		end		
	end
	
	return a
end

return adapter
