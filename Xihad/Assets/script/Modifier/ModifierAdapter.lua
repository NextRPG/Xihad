local ModifierAdapter = {}

function ModifierAdapter.new(modifier)
	local a = {}
	function a.onUpdate(self)
		modifier:update(g_time.change)
	end
	
	return a
end

function ModifierAdapter.fit(gameObject, modifier)
	local updateHandler = ModifierAdapter.new(modifier)
	gameObject:appendUpdateHandler(updateHandler)
	return updateHandler
end

return ModifierAdapter
