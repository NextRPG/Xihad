#pragma once
#include "RoutingTarget.h"
#include "Vec2.h"
#include <unordered_set>

class MaxCostTarget : public RoutingTarget<Vec2, double>
{
public:
	MaxCostTarget(double maxCost) : maxCost(maxCost) {}

	virtual double estimateCostToTarget(Vec2 from)
	{
		return 0;
	}

	virtual NodeCategory classify(Vec2 p, double cost )
	{
		if (cost > maxCost) 
			return RoutingTarget::CULLED;

		all.insert(p);
		return RoutingTarget::EXPANDING;
	}

	double maxCost;
	std::unordered_set<Vec2> all;
};


