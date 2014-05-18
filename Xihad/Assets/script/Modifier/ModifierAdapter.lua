local ModifierAdapter = {}

function ModifierAdapter.new(modifier)
	local a = {}
	function a.onUpdate(self)
		if modifier:update(g_time.change) == 'stop' then
			self:stop()
		end
	end
	
	return a
end

function ModifierAdapter.fit(gameObject, modifier)
	local updateHandler = ModifierAdapter.new(modifier)
	gameObject:appendUpdateHandler(updateHandler)
	return updateHandler
end

return ModifierAdapter
