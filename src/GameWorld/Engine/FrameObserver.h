#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class GameEngine;
	class FrameObserver : public virtual ReferenceCounted
	{
	public:
		virtual void onFrameBegin(GameEngine* source, float nowSecs) = 0;

		virtual void onFrameEnd(GameEngine* source, float nowSecs, float deltaSecs) = 0;
	};
}}

