#pragma once
#include <ctime>

namespace xihad { namespace ngn
{
	float clockToSeconds(clock_t tm)
	{
		return (float) tm / CLOCKS_PER_SEC;
	}

	time_t secondsToClock(float seconds)
	{
		return (time_t) (seconds*CLOCKS_PER_SEC);
	}
}}

