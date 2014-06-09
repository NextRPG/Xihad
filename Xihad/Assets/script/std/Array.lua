local Array = {
	sort = table.sort,
	insert = table.insert,
}

function Array.elements(t)
	local idx = 0
	local function iter(t)
		local size, value = #t, nil
		repeat
			idx = idx + 1
			value = t[idx]
		until value ~= nil or idx >= size
		
		return value
	end
	
	return iter, t
end

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

function Array.empty(t)
	return #t == 0
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
	return table.remove(t)
end

function Array.fastRemoveElementAt(t, i)
	if i ~= #t then
		t[i] = Array.getBack(t)
	end
	
	table.remove(t)
end

function Array.removeElementAt(t, i)
	assert(i, "index required")
	return table.remove(t, i)
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

function Array.reverse(t)
	local low, high = 1, #t
	
	local buf
	while low < high do 
		buf = t[low]
		t[low] = t[high]
		t[high] = buf
		
		low = low + 1
		high = high - 1
	end
	
	return t
end

function Array.copy(src, dst)
	dst = dst or {}
	for _, v in ipairs(src) do
		table.insert(dst, v)
	end
	
	return dst
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

return Array