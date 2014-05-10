local ActionAdapter = {}

function ActionAdapter.new(action)
	local a = {}
	function a.onUpdate(self)
		action:update(g_time.change)
		if action:hasFinished() then
			self:stop()
		end
	end
	
	return a
end

function ActionAdapter.fit(gameObject, action)
	local updateHandler = ActionAdapter.new(action)
	gameObject:appendUpdateHandler(updateHandler)
	return updateHandler
end

return ActionAdapter
