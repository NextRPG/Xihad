#include "CTimerCondition.hpp"
#include <cassert>
#include <cmath>

namespace xihad { namespace dialogue
{
	CTimerCondition::CTimerCondition( float cycle, int times ) :
		mCycle(cycle), mElapsedTime(0), mRestCycle(times)
	{
		assert(mCycle >= 0);
	}


	bool CTimerCondition::satisfy( float delta )
	{
		if (mCycle == 0) return true;
		if (mRestCycle == 0) return false;

		mElapsedTime += delta;
		if (mElapsedTime >= mCycle)
		{
			mElapsedTime -= mCycle;

			if (mRestCycle > 0) 
				--mRestCycle;

			return true;
		}

		return false;
	}

	void CTimerCondition::setCycle( float cycle )
	{
		if (mElapsedTime > cycle)
			mElapsedTime = fmod(mElapsedTime, cycle);

		mCycle = cycle;
	}

}}

