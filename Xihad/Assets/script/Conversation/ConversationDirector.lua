require 'CEGUIBasedDialogue'		-- load dialogue module

local _default_arg = {
	paddingX = 0, paddingY = 0,
	width = 500,  height= 500,
	lineSpace= 0,
}
local ConversationDirector = {
	waitingSpeaker = nil,
	blockedThread  = nil,
	defaultArg = _default_arg,
	impl = nil,
}
ConversationDirector.__index = ConversationDirector

function ConversationDirector.new()
	return setmetatable({
			impl = Conversation.new(),
		}, ConversationDirector)
end

function ConversationDirector:step()
	if not self.waitingSpeaker then 
		return
	end
	
	if self.waitingSpeaker:isTicking() then
		self.waitingSpeaker:speakAll()
	else
		assert(self.blockedThread ~= nil)
		coroutine.resume(self.blockedThread)
	end
end

function ConversationDirector:start()
	local conv = self
	local o = g_scene:pushController({
		onMouseEvent = function(self, event, arg)
			if event.type == "lPressed" then
				conv:step()
			elseif event.type == 'mouseMoved' then
				return 1
			end
			
			return 0
		end
		})
	o:drop()
	
	g_scene:appendUpdateHandler(self.impl)
end

function ConversationDirector:stop()
	g_scene:popController()
	self.impl:requestStop()
end

function ConversationDirector:_create(name, 
		paddingX, paddingY, lineSpace, width, height)
	return self.impl:newSpeaker(name, 
				paddingX, paddingY, lineSpace, width, height)
end

function ConversationDirector:newSpeaker(name, icon, args)
	local defaultArg = self.defaultArg
	
	args = args or defaultArg
	local paddingX  = args.paddingX or defaultArg.paddingX
	local paddingY  = args.paddingY or defaultArg.paddingY
	local lineSpace = args.lineSpace or defaultArg.lineSpace
	local width = args.width or defaultArg.width	
	local height= args.height or defaultArg.height
	
	local speaker = self:_create(name, 
						paddingX, paddingY, lineSpace, width, height)
	
	if icon then
		speaker:setIcon(icon)
	end
	
	return speaker
end

function ConversationDirector:_pre_speak(speaker)
	assert(not self.waitingSpeaker)
	
	speaker:open()
	
	self.waitingSpeaker= speaker
	self.blockedThread = coroutine.running()
end

function ConversationDirector:_after_speak()
	self.waitingSpeaker:deactive()
	self.waitingSpeaker= nil
	self.blockedThread = nil
end

function ConversationDirector:_speak_and_wait(text)
	self.waitingSpeaker:speak(text)
	coroutine.yield()
end

function ConversationDirector:wait(speaker, text)
	self:_pre_speak(speaker)
	self:_speak_and_wait(text)
	self:_after_speak()
end

function ConversationDirector:waitLong(speaker, textArray)
	self:_pre_speak(speaker)
	for _, text in ipairs(textArray) do
		self:_speak_and_wait(text)
	end
	self:_after_speak()
end

return ConversationDirector
