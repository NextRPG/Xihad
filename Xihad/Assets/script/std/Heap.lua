local Array = require 'std.Array'
Heap = {}
Heap.__index = Heap

local function parent(idx) 	return math.floor(idx/2) end
local function left(idx) 	return 2*idx 	end
local function right(idx) 	return 2*idx+1 	end
local function sink(t, i, isAncestor)
	local l = left(i)
	if l > #t then return end
	
	local candi = l
	local r = right(i)
	if r <= #t and not isAncestor(t[l], t[r]) then
		candi = r
	end
	
	if not isAncestor(t[i], t[candi]) then
		local buf = t[i]
		t[i] = t[candi]
		t[candi] = buf
		
		sink(t, candi, isAncestor)
	end
end

local function findHeap(t, v, root, isAncestor)
	if v == t[root] then
		return root
	end
	
	local found = nil
	if left(root) <= #t and isAncestor(t[left(root)], v) then
		found = findHeap(t, v, left(root), isAncestor)
	end
	
	if not found and right(root) <= #t and isAncestor(t[right(root)], v) then
		found = findHeap(t, v, right(root), isAncestor)
	end
	
	return found
end

function Heap.new(comp)
	return setmetatable({
			isAncestor = comp
		}, Heap)
end

Heap.MinHeap = Heap.new(function (a, b) return a<=b end)
Heap.MaxHeap = Heap.new(function (a, b) return a>=b end)

function Heap.sort(t, less)
	less = less or function (a, b) return a<b end
	local maxHeap = Heap.new(function(a, b) return not less(a, b) end)
	maxHeap:build(t)
	local buf = {}
	local len = #t
	for i=1,len do
		buf[len+1-i] = t[1]
		maxHeap:remove(t)
	end
	
	for i=1,len do
		t[i] = buf[i]
	end
end

function Heap:build(t)
	for i = math.floor(#t/2),1,-1 do
		sink(t, i, self.isAncestor)
	end
end

function Heap:remove(t, i)
	i = i or 1
	assert(i >= 1 and i <= #t, "out of heap range")
	
	local element 
	if i == #t then
		element = Array.popBack(t)
	else
		t[i] = t[#t]
		element = Array.popBack(t)
		
		sink(t, i, self.isAncestor)
	end
	
	return element
end

function Heap:insert(t, v)
	local idx = #t + 1
	while idx > 1 and not self.isAncestor(t[parent(idx)], v) do
		t[idx] = t[parent(idx)]
		idx = parent(idx)
	end
	
	t[idx] = v
end

function Heap:find(t, v)
	return findHeap(t, v, 1, self.isAncestor)
end

function Heap:verify(t)
	local maxIdxWithLeftRightChild = math.floor((#t-1)/2)
	for i=1,maxIdxWithLeftRightChild do
		if not self.isAncestor(t[i], t[left(i)]) or not self.isAncestor(t[i], t[right(i)]) then
			return false
		end
	end
	
	local i = maxIdxWithLeftRightChild+1
	if i >= 1 and left(i) <= #t then
		return self.isAncestor(t[i], t[left(i)])
	else
		return true
	end
end

if select('#', ...) == 0 then 
	local function printHeap(a)
		local height = math.ceil(math.log(#a+1)/math.log(2))
		io.write(string.rep('  ', 2^(height-1)))	-- header
		local blank = ''
		local nxt = 1
		for i=1,#a do
			io.write(string.format('%2d', a[i]))
			if i+1 == 2^nxt then
				nxt = nxt + 1
				io.write('\n')
				
				height = height-1
				local header = string.rep('  ', 2^(height-1))
				io.write(header)
				blank = string.rep('  ', 2^height-1)
			else
				io.write(blank)
			end
		end
		print()
	end
	
	local heap = Heap.MaxHeap
	local a = {}
	local len = 31
	for i=1,len do
		Array.insert(a, math.floor(math.random(1, len)))
	end
	heap:build(a)
	
	for i=1,len do
		heap:insert(a, math.floor(math.random(1, len)))
	end
	
	printHeap(a)
	-- test find
	for i,v in ipairs(a) do
		assert(a[heap:find(a, v)] == v)
	end
	
	-- verify heap
	assert(heap:verify(a))

	local half = math.floor(#a/2)
	for i=1,half do
		heap:remove(a, math.floor(math.random(1, half)))
	end	
	heap:remove(a, #a)
	heap:remove(a, 1)
	assert(heap:verify(a))
	printHeap(a)
	heap:remove(a, 1)
	printHeap(a)
	
	Heap.sort(a)
	for i=1,#a-1 do
		assert(a[i] <= a[i+1])
	end
end

return Heap
