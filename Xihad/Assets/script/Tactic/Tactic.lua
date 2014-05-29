local Heap  = require 'std.Heap'
local Class = require 'std.Class'
local CastPair = require 'Tactic.CastPair'
local Location = require 'route.Location'
local functional = require 'std.functional'
local CommandList= require 'Command.CommandList'

local Tactic = {
	castPairs = nil,
}
Tactic.__index = Tactic

function Tactic.new()
	return setmetatable({
			castPairs = {},
		}, Tactic)
end

function Tactic:_encode(skill, impactCenter)
	local pair = CastPair.new(skill, impactCenter)
	local sharedIdx = Heap.MinHeap:find(self.castPairs, pair)
	if not sharedIdx then
		sharedIdx = Heap.MinHeap:insert(self.castPairs, pair)
	end
	
	return self.castPairs[sharedIdx]
end

function Tactic:_createCommandList()
	return CommandList.new()
end

function Tactic:_completeOrder(cmdList)
	error('Tactic: No implementation by default')
end

function Tactic:_try_the_best_to_approach(warrior, location)
	local delegate = Class.copy(warrior)
	
	-- Overerite this function to ignore every enemy to reach the location
	delegate.isLeagueWith = functional.bind1(functional.return_, true)
	
	return g_chessboard:routeRev(delegate, location)
end

function Tactic:_approach(warrior, expectedLocation, reachables)
	if not expectedLocation then
		return warrior:getLocation()
	end
	
	local reversePath = g_chessboard:routeRev(warrior, expectedLocation)
			or self:_try_the_best_to_approach(warrior, expectedLocation)
	
	if reversePath then
		for _, location in ipairs(reversePath) do
			local tile = g_chessboard:getTile(location)
			if reachables[tile] then
				return location
			end
		end
	end
	
	return warrior:getLocation()
end

function Tactic:_get_reachables(warrior)
	local array = g_chessboard:getReachableTiles(warrior)
	local set = {}
	for _, e in ipairs(array) do
		set[e] = true
	end
	
	return set
end

function Tactic:_select_approaching(warrior, enemy, reachables)
	return warrior:getLocation()
end

---
-- @return CommandList
function Tactic:giveOrder()
	local cmdList = self:_createCommandList()
	
	local warrior = self:findPeer(c'Warrior')
	cmdList:setSource(warrior)
	
	local reachables = self:_get_reachables(warrior)
	local enemy, cast, location = self:_completeOrder(warrior, reachables)
	
	if not location then
		location = self:_select_approaching(warrior, enemy, reachables)
		cmdList:setCommand('待机')
	else
		assert(enemy ~= nil and cast ~= nil)
		local cmd, targetLoc = cast:decode()
		cmdList:setCommand('技能', cmd:getName())
		cmdList:setTarget(targetLoc)
	end
	
	assert(location ~= nil)
	cmdList:setLocation(location)
	
	return cmdList
end

return Tactic
