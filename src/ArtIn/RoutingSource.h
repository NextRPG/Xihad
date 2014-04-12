#pragma once

template <typename Node>
class RoutingSource
{
public:
	virtual ~RoutingSource() {}

	virtual unsigned getSourceCount() = 0;
	virtual Node getSource(unsigned index) = 0;
};

