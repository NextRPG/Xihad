#include "Timeline.h"

namespace xihad { namespace ngn
{

	Timeline::Timeline( double initTimeSeconds, double singleStep) :
		mElapsedSeconds(initTimeSeconds),
		mSingleStepSeconds((float) singleStep),
		mDeltaSeconds(0.0f),
		mScale(1.0f),
		mPaused(false)
	{
	}

	void Timeline::update( float dtRealSeconds )
	{
		double dtScaledSeconds = isPaused() ? 0 : dtRealSeconds*getTimeScale();
		incSeconds(dtScaledSeconds);
	}

	void Timeline::singleStep()
	{
		if (isPaused())
		{
			double dtScaledSeconds = mSingleStepSeconds*getTimeScale();
			incSeconds(dtScaledSeconds);
		}
	}

	void Timeline::incSeconds( double scaledTime )
	{
		mDeltaSeconds = static_cast<float> (scaledTime);
		mElapsedSeconds += scaledTime;
	}

	void Timeline::reset( double initTimeSeconds )
	{
		mElapsedSeconds = initTimeSeconds;
		mDeltaSeconds = 0;
	}

}}

