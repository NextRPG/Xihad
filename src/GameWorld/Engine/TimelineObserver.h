#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class GameWorld;
	class TimelineObserver : public virtual ReferenceCounted
	{
	public:
		virtual ~TimelineObserver() {}

		virtual void onTimePaused(GameWorld*) = 0;
		virtual void onTimeResumed(GameWorld*) = 0;
		virtual void onTimeScaled(GameWorld*) = 0;
	};
}}

