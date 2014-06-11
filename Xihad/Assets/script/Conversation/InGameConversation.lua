local Class = require 'std.Class'
local Array = require 'std.Array'
local Algorithm = require 'std.Algorithm'
local ConversationDirector = require 'Conversation.ConversationDirector'
local InGameConversation = {
	delegates= nil,
	director = nil,
	regions  = nil,
	regionDeterminer = nil,
}
InGameConversation.__index = InGameConversation

function InGameConversation.new(determiner)
	assert(determiner)	
	return setmetatable({
			delegates= {},
			director = ConversationDirector.new(),
			regions  = {},
			regionDeterminer = determiner,
		}, InGameConversation)
end

function InGameConversation:setDefaultCreationArg(arg)
	self.director.defaultArg = arg
end

Class.delegate(InGameConversation, 'start', 'director')

function InGameConversation:stop()
	self.director:stop()
	
	self.delegates= nil
	self.director = nil
	self.regions  = nil
	self.regionDeterminer = nil
end

function InGameConversation:_close_speaker_wait(speaker)
	speaker:close()
end

function InGameConversation:_prepare_region_for(region, forSpeaker)
	local openedSpeaker = self.regions[region]
	if openedSpeaker ~= forSpeaker then
		if openedSpeaker then
			self:_close_speaker_wait(openedSpeaker)
		end
		
		self.regions[region] = forSpeaker
	end
end

function InGameConversation:_close_speaker_except(speaker, except)
	local region = Algorithm.find_t(self.regions, speaker)
	if region ~= nil and region ~= except then
		self:_close_speaker_wait(speaker)
		self.regions[region] = nil	-- remove
	end
end

local function getPositionOfDelegate(delegate)
	local worldPos = delegate:getWorldPosition()
	local x, y = g_collision:getScreenCoordFromPosition(worldPos)
	return { x = x, y = y }
end

function InGameConversation:_determine_region(speaker, delegate, restQuery)
	local pos = getPositionOfDelegate(delegate)
	return self.regionDeterminer:updatePosition(speaker, pos, restQuery)
end

function InGameConversation:_wrap_speaker(speaker, delegate, restQuery)
	local wrapped = setmetatable({ impl = speaker }, { __index = speaker })
	
	function wrapped.speak(w, text)
		if type(text) ~= 'string' and type(text) ~= 'table' then
			error('illegal text')
		end
		
		local speaker = w.impl
		local region = self:_determine_region(speaker, delegate, restQuery)
		self:_close_speaker_except(speaker, region)
		self:_prepare_region_for(region, speaker)
		
		if type(text) == 'string' then
			self.director:wait(speaker, text)
		else
			self.director:waitLong(speaker, text)
		end
		
	end
		
	function wrapped.close(w)
		if w.impl:getStatus() ~= 'close' then
			self:_close_speaker_except(w.impl, nil)
		end
	end
	
	return wrapped
end

function InGameConversation:_new_speaker(delegate)
	return self.director:newSpeaker(delegate:getName(), delegate:getIcon())
end

function InGameConversation:_add_delegate(delegate)
	local index = #self.delegates
	table.insert(self.delegates, delegate)
	
	local restQuery = {
		getCount = function(restSelf)
			return #self.delegates - 1
		end,
		
		getPosition = function(restSelf, idx)
			if idx >= index then
				idx = idx + 1
			end
			
			return getPositionOfDelegate(self.delegates[idx])
		end
	}
	
	return restQuery
end

function InGameConversation:_check_unique(delegate)
	if Array.find(self.delegates, delegate) then
		error('Duplicated delegate')
	end
end

function InGameConversation:addSpeaker(delegate)
	self:_check_unique(delegate)
	
	local restQuery = self:_add_delegate(delegate)
	local speakerImpl = self:_new_speaker(delegate)
	return self:_wrap_speaker(speakerImpl, delegate, restQuery)
end

return InGameConversation