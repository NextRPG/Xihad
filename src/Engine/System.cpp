#include "System.h"

#ifdef _WIN32

#include <windows.h>

inline static double getFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	return (double)frequency.QuadPart;
}

namespace xihad { namespace ngn
{
	double System::frequency = getFrequency();

	double System::getCurrentTime()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return (double)counter.QuadPart / frequency;
	}

}}

#else

#error "No clock implementation for current platform now"

#endif