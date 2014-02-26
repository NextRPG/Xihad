require "math3d"
require "mathplus"
require "tableplus"
require "coordinate"

-- function

function runCallback( result, func, t )
	if type(func) == "function" then		
		func(unpack(t))
	end
	if result ~= true then print(func) end
end

function runAsyncFunc( func, ... )

	local current = coroutine.running()
	local t = {...}
	table.insert(t, function (  )
		runCallback(coroutine.resume(current))
	end)
	
	coroutine.yield(func, t)
end

function findMax( list )
	local maxKey, maxValue = next(list)
	for k,v in pairs(list) do
		if maxValue < v then
			maxKey, maxValue = k, v
		end	
	end
	return maxKey, maxValue
end

function findMin( list )
	local minKey, minValue = next(list)
	for k,v in pairs(list) do
		if minValue > v then
			minKey, minValue = k, v
		end
	end
	return minKey, minValue
end

