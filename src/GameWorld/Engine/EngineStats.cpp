#include "EngineStats.h"
#include <irrlicht/IVideoDriver.h>

namespace xihad { namespace ngn
{

	EngineStats::EngineStats( irr::video::IVideoDriver* driver ) :
		mDriver(driver), mTotalElapsedTime(0), mTotalFrameCount(0)
	{

	}

	void EngineStats::recordFrame( float stepTime )
	{
		++mTotalFrameCount;
		mTotalElapsedTime += stepTime;
	}

	double EngineStats::getAverageFPS() const
	{
		if (mTotalElapsedTime == 0) return 1;

		return(mTotalFrameCount / mTotalElapsedTime);
	}

	int EngineStats::getRecentFPS() const
	{
		if (!mDriver.get()) return (int) getAverageFPS();
		return mDriver->getFPS();
	}

	double EngineStats::getAveragePrimitiveDrawnRate() const
	{
		if (!mDriver.get()) return 0;
		return mDriver->getPrimitiveCountDrawn(1) / 1000000.0;
	}

	int EngineStats::getLastPrimitiveDrawn() const
	{
		if (!mDriver.get()) return 0;
		return mDriver->getPrimitiveCountDrawn(0);
	}

	void EngineStats::resetDriver( irr::video::IVideoDriver* driver )
	{
		mDriver.reset(driver);
	}

}}

