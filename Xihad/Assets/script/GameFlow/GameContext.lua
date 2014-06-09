local GameContext = {
	scenarioRegistry = nil,
	commandExecutor = nil,
	cameraFacade = nil,
	uiFacade = nil,
}
GameContext.__index = GameContext

function GameContext.new()
	local obj = setmetatable({ }, GameContext)
	return obj
end

return GameContext