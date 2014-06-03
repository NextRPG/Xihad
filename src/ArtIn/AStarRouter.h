#pragma once
#include <queue>
#include <unordered_map>
#include <cassert>
#include "WeightedGraph.h"
#include "RoutingTarget.h"
#include "RoutingSource.h"

template <typename Node, typename Cost, typename Hash = std::hash<Node> >
class AStarRouter
{
private:	// nested types
	typedef WeightedGraph<Node, Cost> GraphT;
	typedef RoutingTarget<Node, Cost> TargetT;		 
	typedef RoutingSource<Node>		  SourceT;

	struct Linkage
	{
		Linkage() {}

		Linkage(Node prev, Node node, Cost cost, TargetT& goal) :
			tail(node), prev(prev), cost(cost), 
			estm(goal.estimateCostToTarget(node) + cost) {}

		// Priority is low if estimate is high
		bool operator<(const Linkage& other) const { return estm > other.estm; }

		Node tail;
		Node prev;
		Cost cost;
		Cost estm;
	};

	typedef std::unordered_map<Node, Linkage, Hash> CloseMap;

	typedef std::priority_queue<Linkage> OpenList;

public:
	AStarRouter(GraphT* graph) : graph(graph), nullnode(graph->getNullNode()) {}

	template <typename ReversePath>
	Cost route(SourceT& source, TargetT& target, ReversePath* outPath = 0)
	{
		OpenList open;
		CloseMap closed;

		initOpenList(open, source, target);
		while (!open.empty())
		{
			Linkage shortestPath = open.top();
			const Node& tail = shortestPath.tail;	// for abbreviate
			open.pop();


			if (closed.count(tail) != 0)
			{
				// There may be two same nodes in OpenList.
				// But we need to keep the first only because of the following assertion.
				// The assertion comes from non-negative edge and steady estimate
				assert(closed.find(tail)->second.cost <= shortestPath.cost);
				continue;
			}

			closed[tail] = shortestPath;
			switch (target.classify(tail, shortestPath.cost))
			{
			case TargetT::PASS:
				if (outPath) constructPath(tail, closed, outPath);
				return closed[tail].cost + graph->getStayCost(tail);
			case TargetT::CULLED:
				continue;
			case TargetT::EXPANDING:
			default:
				break;
			}
			
			
			unsigned size = graph->getAdjacencyCount(tail);
			for (unsigned i = 0; i < size; ++i)
			{
				Node adjacent = graph->getAdjacency(tail, i);
				if (adjacent == tail || adjacent == shortestPath.prev) 
					continue;		// skip loop back edge

				if (adjacent == nullnode) 
					continue;		// skip null node

				Cost addition = graph->computeCost(tail, adjacent);
				if (addition < 0) 
					continue;		// skip negative edge, or it will cause error                

				Cost adjCost  = shortestPath.cost + addition;

				auto adjIter = closed.find(adjacent);
				if (adjIter != closed.end())
				{
					if (adjIter->second.cost <= adjCost)
						continue;	// skip worse path
					
					closed.erase(adjIter);
				}

				open.emplace(tail, adjacent, adjCost, target);
			}
		}

		return Cost();
	}

private:
	template <typename ReversePath>
	void constructPath(Node target, CloseMap& closed, ReversePath* outPath)
	{
		assert(outPath);

		Node visiting = target;
		do 
		{
			outPath->push_back(visiting);
			visiting = closed.at(visiting).prev;
		} while (visiting != nullnode);
	}

	void initOpenList( OpenList& open, SourceT& source, TargetT& target ) 
	{
		unsigned sourceCount = source.getSourceCount();
		for (unsigned i = 0; i < sourceCount; ++i)
		{
			Node start = source.getSource(i);
			if (start == nullnode) 
				continue;

			open.emplace(nullnode, start, graph->getInitCost(start), target);
		}
	}

private:
	GraphT* graph;
	Node nullnode;
};

