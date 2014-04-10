#pragma once

template <typename Node, typename Cost>
class WeightedGraph
{
public:
	virtual ~WeightedGraph() {}

	virtual Node getNullNode() = 0;

	virtual unsigned getAdjacencyCount(Node) = 0;
	virtual Node getAdjacency(Node, unsigned) = 0;

	virtual Cost getInitCost(Node start) = 0;
	virtual Cost computeCost(Node from, Node to) = 0;
	virtual Cost getStayCost(Node target) = 0;
};


