local sCoroutine = {}

local function outputErrorMsg(msg, traceback)
	io.stderr:write("COROUTINE ERR: ", tostring(msg), '\n')
	io.stderr:write(traceback, '\n')
end

function sCoroutine.resume(co, ...)
	local t = { coroutine.resume(co, ...) }
	if t[1] == false then
		outputErrorMsg(t[2], debug.traceback(co))
	else
		table.remove(t, 1)
		return unpack(t)
	end
end

function sCoroutine.start(run, ...)
	sCoroutine.resume(coroutine.create(run), ...)
end

return sCoroutine