#pragma once
#include "Vec2.h"
#include "WeightedGraph.h"

class ChessboardGraph : public WeightedGraph<Vec2, double>
{
public:
	ChessboardGraph(irr::core::dimension2du size);

	virtual Vec2 getNullNode();

	virtual unsigned getAdjacencyCount( Vec2 v );
	virtual Vec2 getAdjacency( Vec2 v, unsigned idx );

private:
	irr::core::dimension2du size;
};


