local InGameConversation = {
	warriors = nil,
	director = nil,
	regions  = nil,
	regionDeterminer = nil,
}
InGameConversation.__index = InGameConversation

function InGameConversation.new()
	return setmetatable({
			warriors = {},
			director = nil,			-- TODO
			regions  = {},
			regionDeterminer = nil,	-- TODO
		}, InGameConversation)
end

function InGameConversation:addWarrior(warrior)
	assert(not self.warriors[warrior], 'Already added')
	
	local speaker = self.director:newSpeaker(warrior:getName())
	
	-- function speaker:speak()
	-- 	determine region, close other if needed, speak, deactive
	-- end
	-- function speaker:close()
	--	notify region,
	-- end
	-- return wrapped speaker
end

return InGameConversation