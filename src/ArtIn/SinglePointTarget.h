#pragma once
#include "AStarRouter.h"
#include "ChessboardGraph.h"

class SinglePointTarget : public RoutingTarget<Vec2, double>
{
public:
	SinglePointTarget(int goalX, int goalY) : 
		goal(goalX, goalY), estimateFactor(1) {}

	void setEstimateFactor(double factor) 
	{
		estimateFactor = factor;
	}

	virtual double estimateCostToTarget( Vec2 from );
	
	virtual NodeCategory classify(Vec2 v, double cost);
	
private:
	Vec2 goal;
	double estimateFactor;
};
