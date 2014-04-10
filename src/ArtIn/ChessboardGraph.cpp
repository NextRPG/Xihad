#include "ChessboardGraph.h"

ChessboardGraph::ChessboardGraph(irr::core::dimension2du size) : size(size) {}

Vec2 ChessboardGraph::getNullNode() { return Vec2(); }

unsigned ChessboardGraph::getAdjacencyCount( Vec2 v )
{
	int cnt = 4;
	if (v.X == 1) --cnt;
	if (v.Y == 1) --cnt;
	if (v.X == size.Width) --cnt;
	if (v.Y == size.Height) --cnt;

	return cnt;
}

Vec2 ChessboardGraph::getAdjacency( Vec2 v, unsigned idx )
{
	if (v.X == 1) 
		++idx;
	else if (idx == 0)
		return Vec2(v.X-1, v.Y);

	if (v.Y == 1)
		++idx;
	else if (idx == 1)
		return Vec2(v.X, v.Y-1);

	if (v.X == size.Width) 
		++idx;
	else if (idx == 2)
		return Vec2(v.X+1, v.Y);

	if (v.Y == size.Height) 
		++idx;
	else if (idx == 3)
		return Vec2(v.X, v.Y+1);

	return getNullNode();
}

