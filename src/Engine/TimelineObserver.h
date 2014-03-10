#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class GameWorld;
	class TimelineObserver : public virtual ReferenceCounted
	{
	public:
		TimelineObserver() { XIHAD_MLD_NEW_OBJECT; }
		virtual ~TimelineObserver() { XIHAD_MLD_DEL_OBJECT; }

		virtual void onTimePaused(GameWorld*) = 0;
		virtual void onTimeResumed(GameWorld*) = 0;
		virtual void onTimeScaled(GameWorld*) = 0;
	};
}}

