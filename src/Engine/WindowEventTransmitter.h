#pragma once
#include "FrameObserver.h"

namespace xihad { namespace ngn
{
	class WindowEventTransmitter : public FrameObserver
	{
	public:
		WindowEventTransmitter();
		virtual void onFrameBegin( GameEngine* source, float nowSecs );
		virtual void onFrameEnd( GameEngine* source, float nowSecs, float deltaSecs );
	};
}}

