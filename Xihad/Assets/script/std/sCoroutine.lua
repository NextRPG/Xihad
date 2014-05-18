local sCoroutine = {}

local function outputErrorMsg(msg)
	io.stderr:write("coroutine resume failed\r\n\t", msg, '\n')
	io.stderr:write(debug.traceback())
	io.stderr:flush()
end

function sCoroutine.resume(co, ...)
	local t = { coroutine.resume(co, ...) }
	if t[1] == false then
		outputErrorMsg(t[2])
	else
		table.remove(t, 1)
		return unpack(t)
	end
end

return sCoroutine