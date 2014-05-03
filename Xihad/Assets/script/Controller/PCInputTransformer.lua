local PCInputTransformer = {
	receiver = nil,
}

-- Event receiver must deepcopy
function PCInputTransformer.new(receiver)
	local o = {}
	o.receiver = receiver
	o.onMouseEvent = PCInputTransformer.onMouseEvent
	o.onKeyUp = PCInputTransformer.onKeyUp
	o.onKeyDown = PCInputTransformer.onKeyDown
	return o
end

function PCInputTransformer:onMouseEvent( e )
	local receiver = self.receiver
	
	if e.type == 'rPressed' then
		receiver:onBack()
	elseif e.type == 'lPressed' then
		receiver:onTouch(g_cursor:getPosition())
	elseif e.type == 'mouseMoved' then
		if not e.leftPressed and not e.rightPressed then
			receiver:onHover(g_cursor:getPosition())
		end
	end
	
	-- TODO need to return non-zero if not handled?
	return 0
end

function PCInputTransformer:onKeyUp( e )
	if e.key == '1' then
		self.receiver:onUICommand('待机')
	elseif e.key == '2' then
		self.receiver:onUICommand('烈火')
	end
		
	return 1
end

return PCInputTransformer
