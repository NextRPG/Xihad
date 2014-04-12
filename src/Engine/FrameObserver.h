#pragma once
#include "CppBase/ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class GameEngine;
	class FrameObserver : public virtual ReferenceCounted
	{
	public:
		FrameObserver() { XIHAD_MLD_NEW_OBJECT; }

		virtual ~FrameObserver() { XIHAD_MLD_DEL_OBJECT; }

		virtual void onFrameBegin(GameEngine* source, float nowSecs) = 0;

		virtual void onFrameEnd(GameEngine* source, float nowSecs, float deltaSecs) = 0;
	};
}}

