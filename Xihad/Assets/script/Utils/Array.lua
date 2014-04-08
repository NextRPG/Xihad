Array = {
	sort = table.sort,
	insert = table.insert,
}

function Array.find(t, v, pos)
	pos = pos or 1
	pos = math.max(1, pos)
	
	for i = pos,#t do
		if t[i] == v then
			return i
		end
	end
end

function Array.findLast(t, v, last)
	last = last or #t+1
	
	for i = last-1,1,-1 do
		if t[i] == v then
			return i
		end
	end
end

function Array.getFront(t)
	return t[1]
end

function Array.getBack(t)
	return t[#t]
end

function Array.pushBack(t, v)
	table.insert(t, v)
end

function Array.popBack(t)
	table.remove(t)
end

function Array.removeElementAt(t, i)
	assert(i, "index required")
	table.remove(t, i)
end

function Array.removeElement(t, v)
	local idx = Array.find(t, v)
	
	if idx then
		Array.removeElementAt(t, idx)
		return true
	end
	
	return false
end

function Array.removeElements(t, v)
	local idx = Array.findLast(t, v)
	
	if not idx then return false end
	
	while idx do
		Array.removeElementAt(t, idx)
		idx = Array.findLast(t, v, idx)
	end
	return true
end

function Array.bsearch(t, v, comp)
	local low  = 1
	local high = #t+1
	comp = comp or function(a, b) return a < b end
	
	while low ~= high do
		local mid = math.floor((low+high)/2)
		assert(mid>=low and mid<high)
		
		if t[mid] < v then
			low = mid+1
		elseif t[mid] > v then 
			high = mid
		else
			return mid
		end
	end
	
	-- assert(t[high-1] < v and v < t[high])
	return -high
end

if select('#', ...) == 0 then 
	local arr = { 12, 87, 123, 53, 58, 992, 100 }
	
	Array.sort(arr)
	for i,v in ipairs(arr) do
		assert(Array.bsearch(arr, v) == i)
	end
	
	-- 保持有序性
	Array.insert(arr, Array.bsearch(arr, 58), 58)
	Array.insert(arr, -Array.bsearch(arr, 59), 59)
	local prev = 0
	for i,v in ipairs(arr) do
		assert(prev <= v)
	end
	
	assert(Array.bsearch(arr, 999) == -(#arr+1))
	assert(Array.bsearch({}, 1) == -1)
end

