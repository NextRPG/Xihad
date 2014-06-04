require 'CEGUIBasedDialogue'		-- load dialogue module

local ConversationDirector = {
	waitingSpeaker = nil,
	blockedThread  = nil,
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
	
	-- g_scheduler:schedule(function ()
		self.impl:stop()
	-- end, 0.5)
end

local defaultArg = {
	paddingX = 0, paddingY = 0,
	width = 500,  height= 500,
	lineSpace= 0,
}
function ConversationDirector:newSpeaker(name, args)
	args = args or defaultArg
	local paddingX  = args.paddingX or defaultArg.paddingX
	local paddingY  = args.paddingY or defaultArg.paddingY
	local lineSpace = args.lineSpace or defaultArg.lineSpace
	local width = args.width or defaultArg.width	
	local height= args.height or defaultArg.height
	
	return self.impl:newSpeaker(name, 
				paddingX, paddingY, lineSpace, width, height)
end

function ConversationDirector:wait(speaker, text)
	speaker:open()
	speaker:active()
	speaker:speak(text)
	self.waitingSpeaker= speaker
	self.blockedThread = coroutine.running()
	coroutine.yield()
	-- speaker:close()
end

return ConversationDirector
