require 'Utils.Array'

local function weightedPoint(position, prev, cost, predict)
	return {
		position= position,
		prev	= prev,
		cost 	= cost,
		predict = predict,
	}
end

-- edges must return the same object for hash
function search(start, query)
	local closed = {}
	local open = PriorityQueue.new()
	open:insert(weightedPoint(position, nil, 0, query:predict(start)))
	
	local finish = nil
	while not open:empty() do
		local minWeightedPoint = open:extractMin()
		local current = minWeightedPoint.position
		
		assert(not closed[current])
		closed[current] = minWeightedPoint
		
		if query:isGoal(current) then 
			finish = current
			break 
		end
		
		for adjacency in query:edges(current) do
			local addition = query:cost(current, adjacency)
			local newCost = minWeightedPoint.cost + addition
			
			if not closed[adjacency] or closed[adjacency].cost > newCost then
				closed[adjacency] = nil
				local predict = query:predict(adjacency)
				open:insert(weightedPoint(adjacency, current, newCost, newCost+predict))
			end
		end
	end
	
	if finish then
		-- Got it!
		local reversePath = { finish }
		while Array.getBack(reversePath) ~= start do
			table.insert(reversePath, closed[Array.getBack(reversePath)].prev)
		end
		
		return reversePath
	end
end
