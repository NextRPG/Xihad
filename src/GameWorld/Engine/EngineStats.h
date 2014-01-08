#pragma once
#include "irr_ptr.h"

namespace irr { namespace video 
{
	class IVideoDriver;
}}

namespace xihad { namespace ngn
{
	class EngineStats
	{
	public:
		explicit EngineStats(irr::video::IVideoDriver* driver);

		double getAverageFPS() const;

		int getRecentFPS() const;

		double getAveragePrimitiveDrawnRate() const;

		int getLastPrimitiveDrawn() const;

		void recordFrame(float stepTime);

		void resetDriver(irr::video::IVideoDriver* driver);

	private:
		irr_ptr<irr::video::IVideoDriver> mDriver;
		double mTotalElapsedTime;
		int mTotalFrameCount;
	};
}}
