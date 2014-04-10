#include "SinglePointTarget.h"

double SinglePointTarget::estimateCostToTarget( Vec2 from )
{
	double dx = goal.X - from.X;
	double dy = goal.Y - from.Y;
	return (abs(dx) + abs(dy))*estimateFactor;
}

SinglePointTarget::NodeCategory SinglePointTarget::classify( Vec2 v, double cost )
{
	if (goal.X == v.X && goal.Y == v.Y)
		return PASS;

	return EXPANDING;
}
