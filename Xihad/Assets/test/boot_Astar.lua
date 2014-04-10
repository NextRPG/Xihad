require 'ArtIn'

local delegate = {}
function delegate:computeCost(x1, y1, x2, y2)
	--print(x1, y1, x2, y2)
	return 1
end

local graph = Astar.newChessboard(5, 5, delegate)
local source = Astar.newSinglePointSource(1, 1)
local target = Astar.newSinglePointTarget(5, 5)
local reversePath = {}
local cost = Astar.route(source, target, graph, reversePath)


print('cost', cost)
print('reversePath:')
for i=1,#reversePath, 2 do
	print(reversePath[i], reversePath[i+1])
end

io.read()
