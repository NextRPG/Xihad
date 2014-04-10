#pragma once

template <typename Node, typename Cost>
class RoutingTarget
{
public:
	enum NodeCategory { PASS, EXPANDING, CULLED };

public:
	virtual ~RoutingTarget() {}
	virtual Cost estimateCostToTarget(Node from) = 0;
	virtual NodeCategory classify(Node, Cost) = 0;
};
