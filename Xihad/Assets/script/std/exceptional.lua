local exceptional = {}

function exceptional.call(t)
	assert(type(t.try) == 'function')
	local success, exception = pcall(t.try)
	if not success and t.catch then
		success, exception = pcall(t.catch, exception)
	end
	
	if t.finally then
		--- TODO
		-- What should happended if finally chunk throws another exception?
		t.finally()
	end
	
	if not success then
		error(exception)
	end
end

function exceptional.callself(object, fname, ...)
	return pcall(object[fname], object, ...)
end

return exceptional
