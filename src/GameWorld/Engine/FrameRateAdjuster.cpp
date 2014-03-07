#include "FrameRateAdjuster.h"
#include "GameEngine.h"
#include "Process.h"

namespace xihad { namespace ngn
{

	void FrameRateAdjuster::onFrameBegin( GameEngine* source, float nowSecs )
	{
	}

	void FrameRateAdjuster::onFrameEnd( GameEngine* source, float nowSecs, float delta )
	{
		if (delta < minInterval)
		{
			Process::sleep(minInterval-delta);
			delta = minInterval;
		}
		else if (delta >= debugThreshold)
		{	// in here only when debugThreshold > minInterval
			delta = minInterval;
		}

		source->setFrameTime(delta);
	}

}}

