#pragma once
#include "FrameObserver.h"
#include <ctime>	// for clock_t
#include <utility>

namespace xihad { namespace ngn
{
	class FrameRateAdjuster : public FrameObserver
	{
	public:
		FrameRateAdjuster(float supposedUpdateInterval = 1.f/60, float debugThreshold = 1.f);

		float getMinUpdateInterval() const { return minInterval;}

		/// 如果单次循环所花费事件小于该值，则会让进程休眠直至满足条件
		void setMinUpdateInterval(float newInterval) { minInterval = newInterval; }

		/// 取得断点临界时间
		/**
		 * @see setBreakPointThresholdTime()
		 */
		float getDebugThreshold() const { return debugThreshold; }

		/// 设置断点临界时间
		/**
		 * 如果单次循环的时间超过这个临界值，那么被视为触发了某个断点。下一次更新的间隔会是最小
		 * 更新时间。
		 * 
		 * @see setMinUpdateInterval
		 */
		void setDebugThreshold(float threshold) { debugThreshold = threshold; }

		virtual void onFrameBegin( GameEngine* source, float nowSecs );

		virtual void onFrameEnd( GameEngine* source, float nowSecs, float delta );

	private:
		float minInterval;
		float debugThreshold;
	};
}}

