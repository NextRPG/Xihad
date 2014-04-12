#pragma once
#include "RoutingSource.h"

template <typename Node>
class SinglePointSource : public RoutingSource<Node>
{
public:
	SinglePointSource(Node source) : source(source) {}

	virtual unsigned getSourceCount() 
	{
		return 1;
	}

	virtual Node getSource( unsigned index ) 
	{
		if (index == 0)
			return source;
		else 
			return Node();
	}



private:
	Node source;
};

