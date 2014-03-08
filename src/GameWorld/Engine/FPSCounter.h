#pragma once
#include "FrameObserver.h"

namespace xihad { namespace ngn
{
	class FPSCounter : public FrameObserver
	{
	public:
		FPSCounter(float fpsUpdateTime = 1.f);

		virtual void onFrameBegin( GameEngine* source, float nowSecs );

		virtual void onFrameEnd( GameEngine* source, float nowSecs, float deltaSecs );

		float getAverageFPS() const { return lastFPSValue; }

		void setFPSUpdateTime(float secs);

	private:
		float lastFPSValue;

		float fpsUpdateTime;
		float recentFPSRecordBegin;
		unsigned recentFPSCount;
	};
}}

