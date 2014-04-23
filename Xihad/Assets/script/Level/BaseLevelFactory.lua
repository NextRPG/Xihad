local Location = require 'route.Location'

local BaseFactory = { 
}
BaseFactory.__index = BaseFactory

function BaseFactory.new()
	local obj = { }
	setmetatable(obj, BaseFactory)
	return obj
end

function BaseFactory:createChessboard(width, height)
	error('Not implemented')
end

function BaseFactory:createTerrian(tile, terrainType)
	error('Not implemented')
end

function BaseFactory:createPrefab(team, data)
	error('Not implemented')
end

function BaseFactory:createNamed(team, name)
	error('Not implemented')
end

function BaseFactory:setBarrierTile(barrier, tile)
	barrier:setTile(tile)
end

function BaseFactory:initWarrior(board, object, loc)
	object:resetRotation(math3d.vector(0, loc.rotation, 0))
	
	local barrier = object:findComponent(c'Barrier')
	assert(barrier, 'Barrier component missing')
	
	self:setBarrierTile(barrier, board:getTile(loc))
end

function BaseFactory:create(battle)
	local boardWidth = battle.chessboardWidth
	local boardHeight= #battle.terrain / boardWidth

	-- Create chessboard
	local chessboard = self:createChessboard(boardWidth, boardHeight)
	local boardIndex, boardLocation = 1, Location.new()
	for yCoord = boardWidth, 1, -1 do
		for xCoord = 1, boardHeight do
			boardLocation:set(xCoord, yCoord)
			local tile = chessboard:getTile(boardLocation)
			
			local terrainType = battle.terrain[boardIndex]
			local terrianBarrier = self:createTerrian(terrainType)
			self:setBarrierTile(terrianBarrier, tile)
			
			boardIndex = boardIndex + 1
		end
	end
	
	local character
	for team, teamMembers in pairs(battle.teams) do
		for _, memberInfo in ipairs(teamMembers) do
			local prefabData = battle.prefabs[memberInfo.prefab]
			if prefabData ~= nil then
				character = self:createPrefab(team, prefabData)
			else
				character = self:createNamed(team, memberInfo.prefab)
			end
			
			if character then
				self:initWarrior(chessboard, character, memberInfo)
			end
		end
	end
	
	return chessboard
end

return BaseFactory