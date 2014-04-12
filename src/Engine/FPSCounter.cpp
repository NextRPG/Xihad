#include "FPSCounter.h"
#include <cmath>

namespace xihad { namespace ngn
{

	FPSCounter::FPSCounter( float fpsUpdateTime ) :
		lastFPSValue(0.f), recentFPSRecordBegin(0.f), recentFPSCount(0u)
	{
		setDebugName("FPSCounter");
		setFPSUpdateTime(fpsUpdateTime);
	}


	void FPSCounter::setFPSUpdateTime( float secs )
	{
		if (secs > 0)
			fpsUpdateTime = secs;
	}

	void FPSCounter::onFrameBegin( GameEngine* source, float nowSecs )
	{

	}

	void FPSCounter::onFrameEnd( GameEngine* source, float nowSecs, float deltaSecs )
	{
		float elapsed = nowSecs - recentFPSRecordBegin;
		if (elapsed >= fpsUpdateTime*2)
		{
			lastFPSValue = 0.f;

			recentFPSRecordBegin = nowSecs - fmod(elapsed, fpsUpdateTime);
			recentFPSCount = 0;
		}
		else if (elapsed >= fpsUpdateTime)
		{
			// update fps value
			lastFPSValue = recentFPSCount / fpsUpdateTime;

			// prepare for next iterator
			recentFPSRecordBegin += fpsUpdateTime;
			recentFPSCount = 0;	// reset
		}

		++recentFPSCount;
	}

}}

